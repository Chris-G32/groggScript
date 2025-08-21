#pragma once
#include "abstract_alphabet_node.hpp"

namespace GSAlphabet {

class VariableAssignment : public AbstractAlphabetNode {
public:
    VariableAssignment() {}
    virtual void accept(AbstractAlphabetNodeVisitor* visitor) override;
};

inline void VariableAssignment::accept(AbstractAlphabetNodeVisitor* visitor) {
    visitor->visitVariableAssignment(this);
}

} // namespace GSAlphabet
