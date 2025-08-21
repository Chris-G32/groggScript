#pragma once
#include "abstract_alphabet_node.hpp"

namespace GSAlphabet {

class Statement : public AbstractAlphabetNode {
public:
    Statement() {}
    virtual void accept(AbstractAlphabetNodeVisitor* visitor) override;
};

inline void Statement::accept(AbstractAlphabetNodeVisitor* visitor) {
    visitor->visitStatement(this);
}

} // namespace GSAlphabet
