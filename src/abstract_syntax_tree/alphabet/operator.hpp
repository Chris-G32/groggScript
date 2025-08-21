#pragma once
#include "abstract_alphabet_node.hpp"

namespace GSAlphabet {

class Operator : public AbstractAlphabetNode {
public:
    Operator() {}
    virtual void accept(AbstractAlphabetNodeVisitor* visitor) override;
};

inline void Operator::accept(AbstractAlphabetNodeVisitor* visitor) {
    visitor->visitOperator(this);
}

} // namespace GSAlphabet
