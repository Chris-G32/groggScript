#pragma once
#include <string>
#include <utility>

#include "abstract_alphabet_node.hpp"
#include "primitive.hpp"

namespace GSAlphabet {

class Literal : public AbstractAlphabetNode {
   public:
    explicit Literal(Primitive value, SourceLocation sourceLocation)
        : literal(std::move(value)), location_(sourceLocation) {}
    void accept(AbstractAlphabetNodeVisitor* visitor) override;
    [[nodiscard]] SourceLocation location() const override { return location_; }
    Primitive literal;

   protected:
    SourceLocation location_;
};

inline void Literal::accept(AbstractAlphabetNodeVisitor* visitor) { visitor->visitLiteral(this); }

}  // namespace GSAlphabet
