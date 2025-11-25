#include "interpreter_visitor.hpp"

#include <algorithm>
#include <functional>
#include <iostream>
#include <ostream>
#include <sstream>
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
#include "built_ins/built_in_functions.hpp"
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
gs_value fromPrimitive(Primitive primitive) {
    return std::visit([](const auto& val) -> gs_value { return gs_value(val); },
                      primitive);
}
void throw_expected_non_void_expression() {
    throw std::runtime_error("Expected non-void expression");
}

InterpreterVisitor::InterpreterVisitor() {
    registerNativeFunction(GsBuiltIns::print);
    registerNativeFunction(GsBuiltIns::toString);
}
void InterpreterVisitor::visitProgram(Program* node) {
    visit(node->statements.get());
}
void InterpreterVisitor::visitStatements(Statements* node) {
    for (auto& stmt : node->statements) {
        visit(stmt.get());
        if (_returnFlag) {
            return;
        }
    }
}
void InterpreterVisitor::visitStatement(Statement* node) {
    if (node->type == StatementType::RETURN) {
        if (node->child != nullptr) {
            visit(node->child.get());
        } else {
            _exprResult = std::nullopt;
        }
        _returnFlag = true;
        return;
    }
    visit(node->child.get());
}
void InterpreterVisitor::visitVariableDeclaration(VariableDeclaration* node) {
    if (node->initializer == nullptr) {
        pEnvironment.getDefaultScope().declareSymbol(node->identifier);
        return;
    }
    visit(node->initializer.get());
    pEnvironment.getDefaultScope().initializeSymbol(
        node->identifier, popExpressionResult().value());
}
void InterpreterVisitor::visitVariableAssignment(VariableAssignment* node) {
    visit(node->rhs.get());
    const auto& exprResult = popExpressionResult();
    if (exprResult == std::nullopt) {
        throw std::runtime_error(
            "Expected an evaluatable expression as the rhs");
    }
    pEnvironment.getDefaultScope().assignSymbol(node->identifier, exprResult);
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
    std::optional<gs_value> result = std::nullopt;
    switch (node->op) {
        case LOGICAL_OR:
            // result = lhsVal || rhsVal; TODO: Add logical or
            break;
        case LOGICAL_AND:
            // result = lhsVal && rhsVal; TODO: Add logical and
            break;
        case LOGICAL_EQUALS:
            result = lhsVal == rhsVal;
            break;
        case NOT_EQUALS:
            result = lhsVal != rhsVal;
            break;
        case LESS_THAN:
            result = lhsVal < rhsVal;
            break;
        case LESS_THAN_EQUAL:
            result = lhsVal <= rhsVal;
            break;
        case GREATER_THAN:
            result = lhsVal > rhsVal;
            break;
        case GREATER_THAN_EQUALS:
            result = lhsVal >= rhsVal;
            break;
        case ADDITION:
            result = lhsVal + rhsVal;
            break;
        case SUBTRACTION:
            result = lhsVal - rhsVal;
            break;
        case DIVISION:
            result = lhsVal / rhsVal;
            break;
        case MULTIPLY:
            result = lhsVal * rhsVal;
            break;
        default:
            throw std::logic_error("Unknown binary operator");
    }
    setExprResult(result);
}
void InterpreterVisitor::visitUnaryExpression(UnaryExpression* node) {}
void InterpreterVisitor::visitSymbol(Symbol* node) {
    _exprResult = pEnvironment.getDefaultScope().getSymbol(node->identifier);
}
void InterpreterVisitor::visitLiteral(Literal* node) {
    _exprResult = std::make_optional(fromPrimitive(node->literal));
}
std::optional<gs_value> InterpreterVisitor::popExpressionResult() {
    auto tmp = _exprResult;
    _exprResult = std::nullopt;
    DEBUG_LOG("ExprResult popped:" +
              (tmp.has_value() ? to_string(*tmp) : "null"));
    return tmp;
}
void InterpreterVisitor::visitCallExpression(CallExpression* node) {
    const auto sym = dynamic_cast<Symbol*>(node->callee.get());
    if (sym == nullptr) {
        std::cerr << "Encountered unsupported call expression\n";
        return;
    }
    const std::string identifier = sym->identifier;
    auto binding = pEnvironment.globals.getSymbol(identifier);
    auto& foo = std::get<AbstractGsFunction*>(binding.value().value);
    if (foo == nullptr) {
        throw std::runtime_error(
            "Expected identifier '" + identifier +
            "' to be callable. Received:" + to_string(*binding));
    }
    std::vector<gs_value> args;
    for (const auto& arg : node->arguments) {
        visit(arg.get());
        args.push_back(popExpressionResult().value());
    }

    setExprResult(foo->call(this, args));
    _returnFlag = false;
}
void InterpreterVisitor::visitFunctionDeclaration(FunctionDeclaration* node) {
    auto foo = new GsUserFunction(node->name, node->arguments,
                                  node->returnType.value(), node->body);
    pEnvironment.getDefaultScope().initializeSymbol(node->name, gs_value(foo));
}
void InterpreterVisitor::visitConditionalStatement(ConditionalStatement* node) {
    visit(node->condition.get());
    try {
        const auto result = popExpressionResult();
        if (!result) {
            throw std::runtime_error("No result for evaluated condition.");
        }

        if (result->is_truthy()) {
            visit(node->child.get());
        }
    } catch (const std::bad_variant_access& e) {
        std::cerr << "Expected boolean expression in conditional: " << e.what()
                  << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Unexpected error in conditional statement evaluation: "
                  << e.what() << std::endl;
    }
}
}  // namespace GsInterpreter