#pragma once
#include <string>
#include <utility>

#include "abstract_alphabet_node.hpp"
#include "optional"
namespace GSAlphabet {

class VariableDeclaration : public AbstractAlphabetNode {
   public:
    explicit VariableDeclaration(std::string identifier,
                                 std::unique_ptr<AbstractAlphabetNode> initializer,
                                 SourceLocation sourceLocation)
        : VariableDeclaration(std::move(identifier), std::nullopt, std::move(initializer),
                              sourceLocation) {}

    explicit VariableDeclaration(std::string identifier, std::optional<std::string> type,
                                 std::unique_ptr<AbstractAlphabetNode> initializer,
                                 SourceLocation sourceLocation)
        : type(std::move(type)),
          identifier(std::move(identifier)),
          initializer(std::move(initializer)),
          location_() {}

    void accept(AbstractAlphabetNodeVisitor* visitor) override;
    [[nodiscard]] SourceLocation location() const override { return location_; }
    std::optional<std::string> type;
    std::string identifier;
    std::unique_ptr<AbstractAlphabetNode> initializer;

   protected:
    SourceLocation location_;
};

inline void VariableDeclaration::accept(AbstractAlphabetNodeVisitor* visitor) {
    visitor->visitVariableDeclaration(this);
}

}  // namespace GSAlphabet
