#pragma once
#include "abstract_alphabet_node.hpp"

namespace GSAlphabet {

class Program : public AbstractAlphabetNode {
public:
    Program() {}
    virtual void accept(AbstractAlphabetNodeVisitor* visitor) override;
};

inline void Program::accept(AbstractAlphabetNodeVisitor* visitor) {
    visitor->visitProgram(this);
}

} // namespace GSAlphabet
