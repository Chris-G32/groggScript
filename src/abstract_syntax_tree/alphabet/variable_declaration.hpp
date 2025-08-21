#pragma once
#include "abstract_alphabet_node.hpp"

namespace GSAlphabet {

class VariableDeclaration : public AbstractAlphabetNode {
public:
    VariableDeclaration() {}
    virtual void accept(AbstractAlphabetNodeVisitor* visitor) override;
};

inline void VariableDeclaration::accept(AbstractAlphabetNodeVisitor* visitor) {
    visitor->visitVariableDeclaration(this);
}

} // namespace GSAlphabet
