//
// Created by Christopher Gruska on 12/25/25.
//

#ifndef GROGGSCRIPT_TYPE_CHECKER_HPP
#define GROGGSCRIPT_TYPE_CHECKER_HPP
#include <format>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "../concrete_syntax_tree/alphabet/abstract_alphabet_node.hpp"
#include "type_bindings.hpp"
#include "type_environment.hpp"
namespace GsInterpreter {

class TypeChecker : public GSAlphabet::AbstractAlphabetNodeVisitor {
   public:
    void visitProgram(GSAlphabet::Program* node) override;
    void visitStatements(GSAlphabet::Statements* node) override;
    void visitStatement(GSAlphabet::Statement* node) override;
    void visitVariableDeclaration(GSAlphabet::VariableDeclaration* node) override;
    void visitVariableAssignment(GSAlphabet::VariableAssignment* node) override;
    void visitBinaryExpression(GSAlphabet::BinaryExpression* node) override;
    void visitCallExpression(GSAlphabet::CallExpression* node) override;
    void visitUnaryExpression(GSAlphabet::UnaryExpression* node) override;
    void visitSymbol(GSAlphabet::Symbol* node) override;
    void visitLiteral(GSAlphabet::Literal* node) override;
    void visitFunctionDeclaration(GSAlphabet::FunctionDeclaration* node) override;
    void visitConditionalStatement(GSAlphabet::ConditionalStatement* node) override;
    void visitForLoop(GSAlphabet::ForLoop* node) override;
    ~TypeChecker() override = default;
    const auto& getErrors() { return errors_; }

   private:
    std::string formatSourceLocation(const SourceLocation& loc) {
        return std::format("{}:{}", loc.line, loc.column);
    }
    void addError_(const std::string& errorMessage);
    std::optional<std::string> popTypeResult();
    std::vector<std::string> errors_;
    std::optional<std::string> typeResult_;
    TypeEnvironment env_;
};

}  // namespace GsInterpreter

#endif  // GROGGSCRIPT_TYPE_CHECKER_HPP
