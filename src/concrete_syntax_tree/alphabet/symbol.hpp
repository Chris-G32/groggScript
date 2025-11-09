#pragma once
#include <string>

#include "abstract_alphabet_node.hpp"
namespace GSAlphabet {

class Symbol : public AbstractAlphabetNode {
   public:
    Symbol(const std::string& identifier) : identifier(identifier) {}
    void accept(AbstractAlphabetNodeVisitor* visitor) override;
    std::string identifier;
};

inline void Symbol::accept(AbstractAlphabetNodeVisitor* visitor) {
    visitor->visitSymbol(this);
}

}  // namespace GSAlphabet
