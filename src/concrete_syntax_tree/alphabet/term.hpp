#pragma once
#include "abstract_alphabet_node.hpp"

namespace GSAlphabet {
class Term : public AbstractAlphabetNode {
   public:
    Term(std::unique_ptr<AbstractAlphabetNode> node) : term(std::move(node)) {}
    std::unique_ptr<AbstractAlphabetNode> term;
    void accept(AbstractAlphabetNodeVisitor* visitor) override;
};

inline void Term::accept(AbstractAlphabetNodeVisitor* visitor) {
    visitor->visitTerm(this);
}

}  // namespace GSAlphabet
