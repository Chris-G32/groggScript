#pragma once
#include <vector>

#include "../../lexer/tokens.hpp"
using namespace GroggScript;
class AbstractGrammarVisitor {
   public:
    virtual void visitProgram(class ProgramNode* node) = 0;
    virtual void visitStatements(class Statements* node);
};
class AbstractGrammarNode {
   public:
    virtual ~AbstractGrammarNode() {};
    virtual void accept(AbstractGrammarVisitor* visitor) {
        throw std::runtime_error("NOT IMPLEMENTED");
    };
};
