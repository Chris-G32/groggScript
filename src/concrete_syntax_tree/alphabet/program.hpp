#pragma once
#include <memory>

#include "abstract_alphabet_node.hpp"

namespace GSAlphabet {

class Program : public AbstractAlphabetNode {
   public:
    explicit Program(std::unique_ptr<AbstractAlphabetNode> statements)
        : statements(std::move(statements)) {}
    void accept(AbstractAlphabetNodeVisitor* visitor) override;
    [[nodiscard]] SourceLocation location() const override { return statements->location(); }
    std::unique_ptr<AbstractAlphabetNode> statements;
};

inline void Program::accept(AbstractAlphabetNodeVisitor* visitor) { visitor->visitProgram(this); }

}  // namespace GSAlphabet
