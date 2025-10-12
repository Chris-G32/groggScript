#pragma once
#include <memory>
#include <vector>

#include "abstract_alphabet_node.hpp"

namespace GSAlphabet {

class Statements : public AbstractAlphabetNode {
   public:
    explicit Statements(
        std::vector<std::unique_ptr<AbstractAlphabetNode>> statements)
        : statements(std::move(statements)) {}
    void accept(AbstractAlphabetNodeVisitor* visitor) override {
        visitor->visitStatements(this);
    }
    std::vector<std::unique_ptr<AbstractAlphabetNode>> statements;
};

}  // namespace GSAlphabet
