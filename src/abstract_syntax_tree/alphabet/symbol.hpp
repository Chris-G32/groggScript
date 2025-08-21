#pragma once
#include "abstract_alphabet_node.hpp"

namespace GSAlphabet {

class Symbol : public AbstractAlphabetNode {
public:
    Symbol() {}
    virtual void accept(AbstractAlphabetNodeVisitor* visitor) override;
};

inline void Symbol::accept(AbstractAlphabetNodeVisitor* visitor) {
    visitor->visitSymbol(this);
}

} // namespace GSAlphabet
