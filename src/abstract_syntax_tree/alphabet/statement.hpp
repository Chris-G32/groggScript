#pragma once
#include <memory>

#include "abstract_alphabet_node.hpp"
#include "abstract_expression.hpp"

namespace GSAlphabet {

class Statement : public AbstractAlphabetNode {
   public:
    Statement(std::unique_ptr<AbstractAlphabetNode> statement)
        : child(std::move(statement)) {}
    std::unique_ptr<AbstractAlphabetNode> child;
    virtual void accept(AbstractAlphabetNodeVisitor* visitor) override;
};

inline void Statement::accept(AbstractAlphabetNodeVisitor* visitor) {
    visitor->visitStatement(this);
}

}  // namespace GSAlphabet
