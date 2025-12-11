#ifndef GS_INTERPRETER_VISITOR_HPP
#define GS_INTERPRETER_VISITOR_HPP

#include "../concrete_syntax_tree/alphabet/abstract_alphabet_node.hpp"
#include "../logger/logger.hpp"
#include "environment/interpreter_state.hpp"
#include "functions/gs_native_function.hpp"
#include "functions/gs_user_function.hpp"
#include "gs_value.hpp"
namespace GsInterpreter {

class InterpreterVisitor : public GSAlphabet::AbstractAlphabetNodeVisitor {
   public:
    friend GsUserFunction;
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
    void visitForLoop(GSAlphabet::ForLoop* node) override;
    void visitCallExpression(GSAlphabet::CallExpression* node) override;
    void visitFunctionDeclaration(
        GSAlphabet::FunctionDeclaration* node) override;
    void visitConditionalStatement(
        GSAlphabet::ConditionalStatement* node) override;
    [[nodiscard]] std::optional<gs_value> popExpressionResult();
    void setExprResult(std::optional<gs_value> result) {
        _exprResult = std::move(result);
        DEBUG_LOG("ExprResult set to: " +
                  (_exprResult.has_value() ? to_string(*_exprResult) : "null"));
    }

   private:
    void registerNativeFunction(GsNativeFunction& foo) {
        mEnvironment_.globals.initializeSymbol(foo.name, gs_value(&foo));
    }
    std::optional<gs_value> _exprResult = std::nullopt;
    bool _returnFlag = false;
    InterpreterState mEnvironment_;
};
}  // namespace GsInterpreter
#endif
