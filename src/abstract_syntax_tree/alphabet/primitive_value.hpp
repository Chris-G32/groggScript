#pragma once
#include <string>

#include "abstract_alphabet_node.hpp"
#include "primitive.hpp"
namespace GSAlphabet {

class PrimitiveValue : public AbstractAlphabetNode {
   public:
    PrimitiveValue(const Primitive& value) : value(value) {}
    virtual void accept(AbstractAlphabetNodeVisitor* visitor) override;
    Primitive value;
};

inline void PrimitiveValue::accept(AbstractAlphabetNodeVisitor* visitor) {
    visitor->visitPrimitiveValue(this);
}

}  // namespace GSAlphabet