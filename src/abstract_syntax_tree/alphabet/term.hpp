#pragma once
#include <cassert>

#include "abstract_alphabet_node.hpp"
#include "literal.hpp"
#include "symbol.hpp"
#include "unary_expression.hpp"

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
