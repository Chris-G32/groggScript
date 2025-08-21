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
    virtual void visitExpression(class Expression* node) = 0;
    virtual void visitSymbol(class Symbol* node) = 0;
    virtual void visitPrimitiveValue(class PrimitiveValue* node) = 0;
    virtual void visitOperator(class Operator* node) = 0;
    virtual void visitTerm(class Term* node) = 0;
};

class AbstractAlphabetNode {
   public:
    virtual void accept(AbstractAlphabetNodeVisitor* visitor) = 0;
};

void AbstractAlphabetNodeVisitor::visit(AbstractAlphabetNode* node) {
    node->accept(this);
}
}  // namespace GSAlphabet
