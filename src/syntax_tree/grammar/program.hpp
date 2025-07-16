#pragma once
#include "abstract_statement.hpp"
#include "grammar_node.hpp"
class ProgramNode : public AbstractGrammarNode {
   public:
    ProgramNode() {}
    ProgramNode(Statements statements) : _statements(statements) {}
    virtual void accept(AbstractGrammarVisitor* visitor) override {
        visitor->visitProgram(this);
    }

   protected:
    Statements _statements;
};
