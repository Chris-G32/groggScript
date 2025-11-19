#pragma once
namespace GSAlphabet {

class AbstractAlphabetNodeVisitor {
   public:
    virtual void visit(class AbstractAlphabetNode* node);
    virtual void visitProgram(class Program* node) = 0;
    virtual void visitStatements(class Statements* node) = 0;
    virtual void visitStatement(class Statement* node) = 0;
    virtual void visitVariableDeclaration(class VariableDeclaration* node) = 0;
    virtual void visitVariableAssignment(class VariableAssignment* node) = 0;
    virtual void visitBinaryExpression(class BinaryExpression* node) = 0;
    virtual void visitCallExpression(class CallExpression* node) = 0;
    virtual void visitUnaryExpression(class UnaryExpression* node) = 0;
    virtual void visitSymbol(class Symbol* node) = 0;
    virtual void visitLiteral(class Literal* node) = 0;
    virtual void visitFunctionDeclaration(class FunctionDeclaration* node) = 0;
    virtual void visitConditionalStatement(
        class ConditionalStatement* node) = 0;
    virtual ~AbstractAlphabetNodeVisitor() = default;
};

class AbstractAlphabetNode {
   public:
    virtual ~AbstractAlphabetNode() = default;
    virtual void accept(AbstractAlphabetNodeVisitor* visitor) = 0;
};

inline void AbstractAlphabetNodeVisitor::visit(AbstractAlphabetNode* node) {
    node->accept(this);
}
}  // namespace GSAlphabet
