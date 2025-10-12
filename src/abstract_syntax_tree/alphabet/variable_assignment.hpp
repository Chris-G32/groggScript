#pragma once
#include <string>

#include "abstract_alphabet_node.hpp"

namespace GSAlphabet {

class VariableAssignment : public AbstractAlphabetNode {
   public:
    VariableAssignment(std::string identifier,
                       std::unique_ptr<AbstractAlphabetNode> rhs)
        : identifier(identifier), rhs(std::move(rhs)) {}
    void accept(AbstractAlphabetNodeVisitor* visitor) override;
    std::string identifier;
    std::unique_ptr<AbstractAlphabetNode> rhs;
};

inline void VariableAssignment::accept(AbstractAlphabetNodeVisitor* visitor) {
    visitor->visitVariableAssignment(this);
}

}  // namespace GSAlphabet
