#include "interpreter_visitor.hpp"

#include <algorithm>
#include <iostream>
#include <ostream>
#include <sstream>
#include <unordered_map>
#include <variant>

#include "../concrete_syntax_tree/alphabet/binary_expression.hpp"
#include "../concrete_syntax_tree/alphabet/call_expression.hpp"
#include "../concrete_syntax_tree/alphabet/literal.hpp"
#include "../concrete_syntax_tree/alphabet/primitive.hpp"
#include "../concrete_syntax_tree/alphabet/program.hpp"
#include "../concrete_syntax_tree/alphabet/statement.hpp"
#include "../concrete_syntax_tree/alphabet/statements.hpp"
#include "../concrete_syntax_tree/alphabet/variable_assignment.hpp"
#include "../concrete_syntax_tree/alphabet/variable_declaration.hpp"
#include "../concrete_syntax_tree/printer_visitor.hpp"
#include "functions/gs_native_function.hpp"
using GSAlphabet::BinaryExpression;
using GSAlphabet::BinaryOperator;
using GSAlphabet::Literal;
using GSAlphabet::Primitive;
using GSAlphabet::Program;
using GSAlphabet::Statement;
using GSAlphabet::Statements;
using GSAlphabet::Symbol;
using GSAlphabet::UnaryExpression;
using GSAlphabet::VariableAssignment;
using GSAlphabet::VariableDeclaration;
namespace GsInterpreter {
void throw_expected_non_void_expression() {
    throw std::runtime_error("Expected non-void expression");
}

GSAlphabet::Primitive add(const GSAlphabet::Primitive& lhs,
                          const GSAlphabet::Primitive& rhs) {
    return std::visit(
        [](const auto& lhs, const auto& rhs) -> Primitive {
            using T = std::decay_t<decltype(lhs)>;
            using V = std::decay_t<decltype(rhs)>;
            if constexpr (!std::is_same_v<T, V>) {
                throw std::runtime_error(
                    "Addition is only supported between values of the same "
                    "type.");
            }
            if constexpr (std::is_same_v<T, GSAlphabet::boolean>) {
                throw std::runtime_error(
                    "Arithmetic operations not allowed on type boolean.");
            }
            if constexpr ((std::is_same_v<T, GSAlphabet::integer> &&
                           std::is_same_v<V, GSAlphabet::integer>) ||
                          (std::is_same_v<T, GSAlphabet::decimal> &&
                           std::is_same_v<V, GSAlphabet::decimal>) ||
                          (std::is_same_v<T, GSAlphabet::text> &&
                           std::is_same_v<V, GSAlphabet::text>)) {
                return lhs + rhs;
            }
            throw std::logic_error(
                "Subtraction operation failed, escaped bounds of expected "
                "types.");
        },
        lhs, rhs);
}
GSAlphabet::Primitive subtract(const GSAlphabet::Primitive& lhs,
                               const GSAlphabet::Primitive& rhs) {
    return std::visit(
        [](const auto& lhs, const auto& rhs) -> GSAlphabet::Primitive {
            using T = std::decay_t<decltype(lhs)>;
            using V = std::decay_t<decltype(rhs)>;
            if constexpr (!std::is_same_v<T, V>) {
                throw std::runtime_error(
                    "Subtraction is only supported between values of the same "
                    "type.");
            }
            if constexpr (std::is_same_v<T, GSAlphabet::text>) {
                throw std::runtime_error(
                    "Subtract operation not allowed on type "
                    "string.");
            }
            if constexpr (std::is_same_v<T, GSAlphabet::boolean>) {
                throw std::runtime_error(
                    "Arithmetic operations not allowed on type boolean.");
            }
            if constexpr ((std::is_same_v<T, GSAlphabet::integer> &&
                           std::is_same_v<V, GSAlphabet::integer>) ||
                          (std::is_same_v<T, GSAlphabet::decimal> &&
                           std::is_same_v<V, GSAlphabet::decimal>)) {
                return lhs - rhs;
            }
            throw std::logic_error(
                "Subtraction operation failed, escaped bounds of expected "
                "types.");
        },
        lhs, rhs);
}
InterpreterVisitor::InterpreterVisitor() {
    auto printImpl = [](std::vector<GsValue>& args) -> std::optional<GsValue> {
        std::cout << std::get<text>(std::get<Primitive>(args[0]));
        return std::nullopt;
    };
    auto toStr = [](std::vector<GsValue>& args) -> std::optional<GsValue> {
        return std::visit(
            []<typename T0>(T0& val) {
                using T = std::decay_t<T0>;
                if constexpr (std::is_same_v<T, Primitive>) {
                    return std::visit(
                        [](auto& prim) -> GsValue {
                            std::ostringstream oss;
                            oss << std::boolalpha << prim;
                            return oss.str();
                        },
                        val);

                } else {
                    GsValue retVal = "Function or sum bs";
                    return retVal;
                }
            },
            args[0]);
    };
    AbstractGsFunction* print = new GsNativeFunction(
        "print", {{"text", "string"}}, "string", printImpl);
    _environment._globals.emplace("print", print);
    _environment._globals.emplace(
        "toString",
        new GsNativeFunction("toString", {{"val", "string"}}, "string", toStr));
}
void InterpreterVisitor::visitProgram(Program* node) {
    visit(node->statements.get());
}
void InterpreterVisitor::visitStatements(Statements* node) {
    for (auto& stmt : node->statements) {
        visit(stmt.get());
    }
}
void InterpreterVisitor::visitStatement(Statement* node) {
    if (node->type == StatementType::RETURN) {
        if (node->child != nullptr) {
            visit(node->child.get());
        } else {
            _exprResult = std::nullopt;
        }
    }
    visit(node->child.get());
}
void InterpreterVisitor::visitVariableDeclaration(VariableDeclaration* node) {
    if (node->initializer == nullptr) {
        _environment.declareVariable(node->identifier);
        return;
    }
    visit(node->initializer.get());
    _environment.initializeVariable(node->identifier,
                                    popExpressionResult().value());
}
void InterpreterVisitor::visitVariableAssignment(VariableAssignment* node) {
    visit(node->rhs.get());
    const auto& exprResult = popExpressionResult();
    if (exprResult == std::nullopt) {
        throw std::runtime_error(
            "Expected an evaluatable expression as the rhs");
    }
    _environment.assignVariable(node->identifier, exprResult);
}
void InterpreterVisitor::visitBinaryExpression(BinaryExpression* node) {
    visit(node->left.get());
    auto lhs = popExpressionResult();
    if (!lhs) {
        throw_expected_non_void_expression();
    }
    visit(node->right.get());
    auto rhs = popExpressionResult();
    if (!rhs) {
        throw_expected_non_void_expression();
    }
    const auto& lhsVal = lhs.value();
    const auto& rhsVal = rhs.value();
    switch (node->op) {
        case BinaryOperator::LOGICAL_OR:
            break;
        case BinaryOperator::LOGICAL_AND:
            break;
        case BinaryOperator::LOGICAL_EQUALS:
            break;
        case BinaryOperator::NOT_EQUALS:
            break;
        case BinaryOperator::LESS_THAN:
            break;
        case BinaryOperator::LESS_THAN_EQUAL:
            break;
        case BinaryOperator::GREATER_THAN:
            break;
        case BinaryOperator::GREATER_THAN_EQUALS:
            break;
        case BinaryOperator::ADDITION:
            _exprResult = add(std::get<GSAlphabet::Primitive>(lhsVal),
                              std::get<GSAlphabet::Primitive>(rhsVal));
            break;
        case BinaryOperator::SUBTRACTION:
            _exprResult = subtract(std::get<GSAlphabet::Primitive>(lhsVal),
                                   std::get<GSAlphabet::Primitive>(rhsVal));
        case BinaryOperator::DIVISION:
            break;
        case BinaryOperator::MULTIPLY:
            break;
    }
}
void InterpreterVisitor::visitUnaryExpression(UnaryExpression* node) {}
void InterpreterVisitor::visitSymbol(Symbol* node) {
    _exprResult = _environment.getVariable(node->identifier);
}
void InterpreterVisitor::visitLiteral(Literal* node) {
    _exprResult = std::make_optional(node->literal);
}
const std::optional<GsValue> InterpreterVisitor::popExpressionResult() {
    auto tmp = _exprResult;
    _exprResult = std::nullopt;
    return tmp;
}
void InterpreterVisitor::visitCallExpression(CallExpression* node) {
    const auto sym = dynamic_cast<Symbol*>(node->callee.get());
    if (sym == nullptr) {
        std::cerr << "Encountered unsupported call expression\n";
        return;
    }
    const std::string identifier = sym->identifier;

    if (const auto& foo = _environment._globals.find(identifier);
        foo != _environment._globals.end()) {
        std::vector<GsValue> args;
        for (const auto& arg : node->arguments) {
            visit(arg.get());
            args.push_back(popExpressionResult().value());
        }
        _exprResult =
            std::get<AbstractGsFunction*>(*foo->second)->call(this, args);
    }
}
void InterpreterVisitor::visitFunctionDeclaration(
    GSAlphabet::FunctionDeclaration* node) {
    _environment._globals.emplace(
        node->name, new GsUserFunction(node->name, node->arguments,
                                       node->returnType.value(), node->body));
}
}  // namespace GsInterpreter