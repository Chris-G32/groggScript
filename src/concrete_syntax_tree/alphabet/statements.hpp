#pragma once
#include <memory>
#include <vector>

#include "abstract_alphabet_node.hpp"

namespace GSAlphabet {

class Statements : public AbstractAlphabetNode {
   public:
    explicit Statements(std::vector<std::unique_ptr<AbstractAlphabetNode>> statements)
        : statements(std::move(statements)) {}
    void accept(AbstractAlphabetNodeVisitor* visitor) override { visitor->visitStatements(this); }
    [[nodiscard]] SourceLocation location() const override {
        if (statements.empty()) {
            throw std::invalid_argument("empty statement list has no source location");
        }
        // First statement determines the location for statements
        return statements[0]->location();
    }
    std::vector<std::unique_ptr<AbstractAlphabetNode>> statements;
};

}  // namespace GSAlphabet
