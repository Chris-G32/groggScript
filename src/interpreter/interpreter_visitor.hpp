#pragma once
#include <vector>

#include "../concrete_syntax_tree/alphabet/abstract_alphabet_node.hpp"
#include "environment.hpp"
#include "gs_value.hpp"
namespace GsInterpreter {

class InterpreterVisitor : public GSAlphabet::AbstractAlphabetNodeVisitor {
   public:
    InterpreterVisitor();
    void visitProgram(GSAlphabet::Program* node) override;
    void visitStatements(GSAlphabet::Statements* node) override;
    void visitStatement(GSAlphabet::Statement* node) override;
    void visitVariableDeclaration(
        GSAlphabet::VariableDeclaration* node) override;
    void visitVariableAssignment(GSAlphabet::VariableAssignment* node) override;
    void visitBinaryExpression(GSAlphabet::BinaryExpression* node) override;
    void visitUnaryExpression(GSAlphabet::UnaryExpression* node) override;
    void visitSymbol(GSAlphabet::Symbol* node) override;
    void visitLiteral(GSAlphabet::Literal* node) override;
    void visitTerm(GSAlphabet::Term* node) override;
    [[nodiscard]] const std::optional<GsValue> popExpressionResult();
    void visitCallExpression(GSAlphabet::CallExpression* node) override;
    void visitFunctionDeclaration(
        GSAlphabet::FunctionDeclaration* node) override;

    std::vector<GsValue> outputs;

   private:
    std::optional<GsValue> _exprResult = std::nullopt;
    GsEnvironment _environment;
};
}  // namespace GsInterpreter
