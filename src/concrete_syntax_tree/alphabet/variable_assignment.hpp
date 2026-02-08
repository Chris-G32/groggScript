#pragma once
#include <memory>
#include <string>

#include "abstract_alphabet_node.hpp"

namespace GSAlphabet {

class VariableAssignment : public AbstractAlphabetNode {
   public:
    VariableAssignment(std::string identifier, std::unique_ptr<AbstractAlphabetNode> rhs,
                       const SourceLocation location)
        : identifier(std::move(identifier)), rhs(std::move(rhs)), location_(location) {}
    void accept(AbstractAlphabetNodeVisitor* visitor) override;
    [[nodiscard]] SourceLocation location() const override { return location_; }
    std::string identifier;
    std::unique_ptr<AbstractAlphabetNode> rhs;

   protected:
    SourceLocation location_;
};

inline void VariableAssignment::accept(AbstractAlphabetNodeVisitor* visitor) {
    visitor->visitVariableAssignment(this);
}

}  // namespace GSAlphabet
