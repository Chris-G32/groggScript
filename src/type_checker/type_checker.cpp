#include "type_checker.hpp"

#include <format>
#include <functional>
#include <ranges>

#include "../concrete_syntax_tree/alphabet/array_literal.hpp"
#include "../concrete_syntax_tree/alphabet/function_declaration.hpp"
#include "../concrete_syntax_tree/alphabet/literal.hpp"
#include "../concrete_syntax_tree/alphabet/program.hpp"
#include "../concrete_syntax_tree/alphabet/statement.hpp"
#include "../concrete_syntax_tree/alphabet/statements.hpp"
#include "../concrete_syntax_tree/alphabet/symbol.hpp"
#include "../concrete_syntax_tree/alphabet/unary_expression.hpp"
#include "../concrete_syntax_tree/alphabet/variable_assignment.hpp"
#include "../concrete_syntax_tree/alphabet/variable_declaration.hpp"
#include "../interpreter/gs_value.hpp"
#include "../logger/logger.hpp"
#include "../truth_tables/operator_specifications.hpp"

namespace GsInterpreter {
void TypeChecker::visitProgram(GSAlphabet::Program* node) { visit(node->statements.get()); }
void TypeChecker::visitStatements(GSAlphabet::Statements* node) {
    for (const auto& s : node->statements) {
        visit(s.get());
    }
}
void TypeChecker::visitStatement(GSAlphabet::Statement* node) { visit(node->child.get()); }

void TypeChecker::visitVariableDeclaration(GSAlphabet::VariableDeclaration* node) {
    auto deduceInitializerType =
        [this](GSAlphabet::AbstractAlphabetNode* initNode) -> std::optional<std::string> {
        if (initNode == nullptr) {
            return std::nullopt;
        }
        // Get type of initializer
        visit(initNode);
        if (auto initType = popTypeResult(); initType.has_value()) {
            return initType;
        }
        addError_("Initializers must return a value");
        return std::nullopt;
    };
    auto varType = node->type;
    auto initializerType = deduceInitializerType(node->initializer.get());
    if (initializerType.has_value() && varType.has_value() &&
        initializerType.value() != varType.value()) {
        addError_(
            std::format("Symbol '{}' of type '{}' cannot be initialized with a value of type '{}'",
                        node->identifier, varType.value(), initializerType.value()));
        return;
    }
    if (!initializerType.has_value() && !varType.has_value()) {
        addError_(std::format("Unable to determine a type for symbol '{}'", node->identifier));
        return;
    }
    if (!env_.activeScope().bind(node->identifier,
                                 varType.value_or(initializerType.value_or("")))) {
        addError_(std::format("Redeclaration of symbol '{}'", node->identifier));
    }
}
void TypeChecker::visitVariableAssignment(GSAlphabet::VariableAssignment* node) {
    if (node->rhs != nullptr) {
        visit(node->rhs.get());
    }
    auto identifier = node->identifier;
    if (auto rhsType = popTypeResult(); rhsType.has_value()) {
        if (!env_.activeScope().matches(node->identifier, rhsType.value())) {
            auto lhsType = env_.activeScope().typeOf(node->identifier);
            addError_(
                std::format("Cannot assign value of type '{}' to symbol '{}' which is type '{}'",
                            rhsType.value(), identifier, lhsType));
        }
        return;
    }
    addError_(std::format("Failed to determine type of assignment to '{}'", identifier));
}
void TypeChecker::visitBinaryExpression(GSAlphabet::BinaryExpression* node) {
    using namespace GsTruthTables;
    auto spec = getBySymbol(fromAst(node->op));
    if (node->left == nullptr && node->right == nullptr) {
        addError_(std::format("Operator {} requires a left and a right side",
                              GSAlphabet::toString(node->op)));
        return;
    }
    visit(node->left.get());
    auto leftType = popTypeResult();
    visit(node->right.get());
    auto rightType = popTypeResult();
    if (!rightType.has_value() || !leftType.has_value()) {
        addError_("Expected types for right and left side!");
        return;
    }
    auto view = spec.supportedTypes | std::views::transform([](const auto& val) {
                    auto str1 = to_string(std::get<0>(val));
                    auto str2 = to_string(std::get<1>(val));
                    return std::make_tuple(str1, str2);
                });
    if (const auto entry = std::ranges::find(view, std::make_tuple(*leftType, *rightType));
        entry == view.end()) {
        addError_(std::format(
            "Operator '{}' does not support between '{}' and '{}' at source location '{}'",
            GSAlphabet::toString(node->op), *leftType, *rightType,
            formatSourceLocation(node->location())));
        return;
    }
    typeResult_ = spec.returnType(*leftType, *rightType);
}
void TypeChecker::visitCallExpression(GSAlphabet::CallExpression* node) {
    DEBUG_LOG("Call expression type checking not implemented");
}
void TypeChecker::visitUnaryExpression(GSAlphabet::UnaryExpression* node) {
    DEBUG_LOG("Unary expression type checking not implemented");
    // visit(node->node.get());

    // env_.if (env_.activeScope())
}
void TypeChecker::visitSymbol(GSAlphabet::Symbol* node) {
    if (auto symbolType = env_.activeScope().typeOf(node->identifier);
        symbolType != TypeBindings::UNBOUND_TYPE) {
        typeResult_ = symbolType;
        return;
    }
    addError_(
        std::format("Type can not be deduced for the symbol '{}' has it been "
                    "initialized in scope?",
                    node->identifier));
    typeResult_ = std::nullopt;
}
void TypeChecker::visitLiteral(GSAlphabet::Literal* node) {
    typeResult_ = GSAlphabet::typeKeyword(node->literal);
}
void TypeChecker::visitFunctionDeclaration(GSAlphabet::FunctionDeclaration* node) {
    TypeEnvironmentScopeGuard g(env_);
    if (!node->returnType) {
        DEBUG_LOG("Return type inference not implemented.");
        return;
    }
    if (!env_.activeScope().bind(node->name, *node->returnType)) {
        addError_(std::format("Redeclaration of symbol '{}'", node->name));
    }
}
void TypeChecker::visitConditionalStatement(GSAlphabet::ConditionalStatement* node) {
    DEBUG_LOG("Conditional statement type checking not implemented");
}
void TypeChecker::visitArrayLiteral(GSAlphabet::ArrayLiteral* node) {
    if (node->items.empty()) {
        typeResult_ = std::nullopt;
        return;
    }
    auto it = node->items.begin();
    visit(it->get());
    std::advance(it, 1);
    std::optional<std::string> prevType = typeResult_;

    for (; it != node->items.end(); std::advance(it, 1)) {
        visit(it->get());
        auto currentType = popTypeResult();
        if (currentType != prevType) {
            addError_(std::format("Failed to deduce type of array at {}",
                                  formatSourceLocation(node->location())));
            return;
        }
        prevType = currentType;
    }
    typeResult_ = prevType.value() + "[]";
}
void TypeChecker::visitForLoop(GSAlphabet::ForLoop* node) {
    DEBUG_LOG("For loop type checking not implemented");
}
void TypeChecker::addError_(const std::string& errorMessage) { errors_.push_back(errorMessage); }

std::optional<std::string> TypeChecker::popTypeResult() {
    auto tmp = typeResult_;
    typeResult_ = std::nullopt;
    return tmp;
}
}  // namespace GsInterpreter