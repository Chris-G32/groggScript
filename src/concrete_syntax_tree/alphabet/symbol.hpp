#pragma once
#include <string>
#include <utility>

#include "abstract_alphabet_node.hpp"
namespace GSAlphabet {

class Symbol : public AbstractAlphabetNode {
   public:
    explicit Symbol(std::string identifier, SourceLocation location)
        : identifier(std::move(identifier)), location_(location) {}
    void accept(AbstractAlphabetNodeVisitor* visitor) override;
    [[nodiscard]] SourceLocation location() const override { return location_; }
    std::string identifier;

   protected:
    SourceLocation location_;
};

inline void Symbol::accept(AbstractAlphabetNodeVisitor* visitor) { visitor->visitSymbol(this); }

}  // namespace GSAlphabet
