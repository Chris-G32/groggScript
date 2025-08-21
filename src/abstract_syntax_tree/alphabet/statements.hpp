#pragma once
#include "abstract_alphabet_node.hpp"

namespace GSAlphabet {

class Statements : public AbstractAlphabetNode {
public:
    Statements() {}
    virtual void accept(AbstractAlphabetNodeVisitor* visitor) override;
};

inline void Statements::accept(AbstractAlphabetNodeVisitor* visitor) {
    visitor->visitStatements(this);
}

} // namespace GSAlphabet
