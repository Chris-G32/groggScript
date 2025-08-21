#pragma once
#include "abstract_alphabet_node.hpp"

namespace GSAlphabet {
enum Operators {
    DOUBLE_EQUALS,
    NOT_EQUALS,
    LESS_THAN,
    LESS_THAN_EQUAL,
    GREATER_THAN,
    GREATER_THAN_EQUALS,
    PLUS,
    INCREMENT,
    MINUS,
    DECREMENT,
    DIVIDE,
    TIMES,
    EXPONENT
};

class Operator : public AbstractAlphabetNode {
   public:
    Operator() {}
    virtual void accept(AbstractAlphabetNodeVisitor* visitor) override;
    Operators operation;
};

inline void Operator::accept(AbstractAlphabetNodeVisitor* visitor) {
    visitor->visitOperator(this);
}
}  // namespace GSAlphabet
