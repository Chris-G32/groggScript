#pragma once
#include "grammar_node.hpp"
class AbstractStatement : public AbstractGrammarNode {
   public:
    virtual string nodeName() = 0;
};
class Statements : public AbstractGrammarNode {
   public:
    Statements() {};
    Statements(const std::vector<AbstractStatement *> &statements)
        : _statements(statements) {}

   protected:
    std::vector<AbstractStatement *> _statements;
};
