//
// Created by Christopher Gruska on 12/25/25.
//

#include "type_checker.hpp"

#include <format>
#include <functional>
#include <ranges>

#include "../concrete_syntax_tree/alphabet/function_declaration.hpp"
#include "../concrete_syntax_tree/alphabet/literal.hpp"
#include "../concrete_syntax_tree/alphabet/statement.hpp"
#include "../concrete_syntax_tree/alphabet/statements.hpp"
#include "../concrete_syntax_tree/alphabet/symbol.hpp"
#include "../concrete_syntax_tree/alphabet/unary_expression.hpp"
#include "../concrete_syntax_tree/alphabet/variable_declaration.hpp"
#include "../interpreter/gs_value.hpp"
#include "../logger/logger.hpp"
#include "../truth_tables/operator_specifications.hpp"

namespace GsInterpreter {
void TypeChecker::visitProgram(GSAlphabet::Program* node) {}
void TypeChecker::visitStatements(GSAlphabet::Statements* node) {
    for (const auto& s : node->statements) {
        visit(s.get());
    }
}
void TypeChecker::visitStatement(GSAlphabet::Statement* node) { if (node->type) }
void TypeChecker::visitVariableDeclaration(GSAlphabet::VariableDeclaration* node) {
    if (!node->type) {
        DEBUG_LOG("Return type inference not implemented.");
        return;
    }
    if (!env_.activeScope().bind(node->identifier, *node->type)) {
        addError_(std::format("Redeclaration of symbol '{}'", node->identifier));
    }
}
void TypeChecker::visitVariableAssignment(GSAlphabet::VariableAssignment* node) {}
void TypeChecker::visitBinaryExpression(GSAlphabet::BinaryExpression* node) {
    using namespace GsTruthTables;
    auto spec = getBySymbol(fromAst(node->op));
    if (node->left != nullptr && node->right != nullptr) {
        addError_(std::format("Operator {} requires a left and a right side.",
                              GSAlphabet::toString(node->op)));
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
    auto entry = std::ranges::find(view, std::make_tuple(*leftType, *rightType));
    if (entry == view.end()) {
        addError_(std::format("Operator {} does not support between {} and {}.",
                              GSAlphabet::toString(node->op), *leftType, *rightType));
    }
}
void TypeChecker::visitCallExpression(GSAlphabet::CallExpression* node) {}
void TypeChecker::visitUnaryExpression(GSAlphabet::UnaryExpression* node) {
    visit(node->node.get());

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
void TypeChecker::visitConditionalStatement(GSAlphabet::ConditionalStatement* node) {}
void TypeChecker::visitForLoop(GSAlphabet::ForLoop* node) {}
void TypeChecker::addError_(const std::string& errorMessage) { errors_.push_back(errorMessage); }

std::optional<std::string> TypeChecker::popTypeResult() {
    auto tmp = typeResult_;
    typeResult_ = std::nullopt;
    return tmp;
}
}  // namespace GsInterpreter