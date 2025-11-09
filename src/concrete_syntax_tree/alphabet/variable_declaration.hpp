#pragma once
#include <string>
#include <utility>

#include "abstract_alphabet_node.hpp"
#include "optional"
namespace GSAlphabet {

class VariableDeclaration : public AbstractAlphabetNode {
   public:
    explicit VariableDeclaration(
        std::string identifier,
        std::unique_ptr<AbstractAlphabetNode> initializer = nullptr)
        : VariableDeclaration(std::move(identifier), std::nullopt,
                              std::move(initializer)) {}

    explicit VariableDeclaration(
        std::string identifier, std::optional<std::string> type,
        std::unique_ptr<AbstractAlphabetNode> initializer = nullptr)
        : type(std::move(type)),
          identifier(std::move(identifier)),
          initializer(std::move(initializer)) {}

    void accept(AbstractAlphabetNodeVisitor* visitor) override;
    std::optional<std::string> type;
    std::string identifier;
    std::unique_ptr<AbstractAlphabetNode> initializer;
};

inline void VariableDeclaration::accept(AbstractAlphabetNodeVisitor* visitor) {
    visitor->visitVariableDeclaration(this);
}

}  // namespace GSAlphabet
