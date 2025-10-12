#pragma once
#include <string>

#include "abstract_alphabet_node.hpp"
#include "primitive.hpp"

namespace GSAlphabet {

class Literal : public AbstractAlphabetNode {
   public:
    Literal(const Primitive& value) : literal(value) {}
    virtual void accept(AbstractAlphabetNodeVisitor* visitor) override;
    Primitive literal;
};

inline void Literal::accept(AbstractAlphabetNodeVisitor* visitor) {
    visitor->visitLiteral(this);
}

}  // namespace GSAlphabet
