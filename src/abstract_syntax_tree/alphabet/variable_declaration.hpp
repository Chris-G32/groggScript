#pragma once
#include <string>

#include "abstract_alphabet_node.hpp"

namespace GSAlphabet {

class VariableDeclaration : public AbstractAlphabetNode {
   public:
    VariableDeclaration(
        const std::string& type, const std::string& identifier,
        std::unique_ptr<AbstractAlphabetNode> initializer = nullptr)
        : type(type),
          identifier(identifier),
          initializer(std::move(initializer)) {}
    void accept(AbstractAlphabetNodeVisitor* visitor) override;
    std::string type;
    std::string identifier;
    std::unique_ptr<AbstractAlphabetNode> initializer;
};

inline void VariableDeclaration::accept(AbstractAlphabetNodeVisitor* visitor) {
    visitor->visitVariableDeclaration(this);
}

}  // namespace GSAlphabet
