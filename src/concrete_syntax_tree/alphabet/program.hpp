#pragma once
#include <memory>

#include "abstract_alphabet_node.hpp"

namespace GSAlphabet {

class Program : public AbstractAlphabetNode {
   public:
    Program(std::unique_ptr<AbstractAlphabetNode> statements)
        : statements(std::move(statements)) {}
    virtual void accept(AbstractAlphabetNodeVisitor* visitor) override;
    std::unique_ptr<AbstractAlphabetNode> statements;
};

inline void Program::accept(AbstractAlphabetNodeVisitor* visitor) {
    visitor->visitProgram(this);
}

}  // namespace GSAlphabet
