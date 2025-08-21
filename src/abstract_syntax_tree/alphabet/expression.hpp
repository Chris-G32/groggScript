#pragma once
#include "abstract_alphabet_node.hpp"

namespace GSAlphabet {

class Expression : public AbstractAlphabetNode {
public:
    Expression() {}
    virtual void accept(AbstractAlphabetNodeVisitor* visitor) override;
};

inline void Expression::accept(AbstractAlphabetNodeVisitor* visitor) {
    visitor->visitExpression(this);
}

} // namespace GSAlphabet
