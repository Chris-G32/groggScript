#pragma once
#include "abstract_alphabet_node.hpp"

namespace GSAlphabet {

class PrimitiveValue : public AbstractAlphabetNode {
public:
    PrimitiveValue() {}
    virtual void accept(AbstractAlphabetNodeVisitor* visitor) override;
};

inline void PrimitiveValue::accept(AbstractAlphabetNodeVisitor* visitor) {
    visitor->visitPrimitiveValue(this);
}

} // namespace GSAlphabet
