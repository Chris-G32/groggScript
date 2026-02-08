#pragma once
#include "grammar_node.hpp"
#include "grammar_errors.hpp"
#include "tokens.hpp"
#include <stdexcept>
class Expression : public AbstractGrammarNode
{
protected:
    Token _symbolOrPrimitive;
    Token _operation;
    Expression *_rhs;

public:
    Expression(Token symbolOrPrimitive) : _symbolOrPrimitive(symbolOrPrimitive)
    {
        throw std::runtime_error("NOT IMPLEMENTED");
    }
    Expression(Token symbolOrPrimitive, Token operation, Expression expression) : Expression(symbolOrPrimitive)
    {
    }
};
