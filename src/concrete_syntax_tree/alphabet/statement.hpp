#pragma once
#include <memory>

#include "abstract_alphabet_node.hpp"
#include "call_expression.hpp"

namespace GSAlphabet {
enum class StatementType { ONE_LINE, RETURN };
class Statement : public AbstractAlphabetNode {
   public:
    explicit Statement(std::unique_ptr<AbstractAlphabetNode> statement,
                       const StatementType type = StatementType::ONE_LINE)
        : child(std::move(statement)), type(type) {}
    std::unique_ptr<AbstractAlphabetNode> child;
    StatementType type;
    void accept(AbstractAlphabetNodeVisitor* visitor) override;
    [[nodiscard]] SourceLocation location() const override { return child->location(); }
};

inline void Statement::accept(AbstractAlphabetNodeVisitor* visitor) {
    visitor->visitStatement(this);
}

}  // namespace GSAlphabet
