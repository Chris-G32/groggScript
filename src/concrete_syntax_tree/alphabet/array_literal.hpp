#ifndef GROGGSCRIPT_ARRAY_LITERAL_HPP
#define GROGGSCRIPT_ARRAY_LITERAL_HPP
#include <memory>
#include <vector>

#include "abstract_alphabet_node.hpp"

namespace GSAlphabet {
class ArrayLiteral : public AbstractAlphabetNode {
   public:
    explicit ArrayLiteral(std::vector<std::unique_ptr<AbstractAlphabetNode>> items,
                          SourceLocation loc)
        : items(std::move(items)), location_(loc) {}
    void accept(AbstractAlphabetNodeVisitor* visitor) override { visitor->visitArrayLiteral(this); }
    [[nodiscard]] SourceLocation location() const override { return location_; }
    std::vector<std::unique_ptr<AbstractAlphabetNode>> items;

   protected:
    SourceLocation location_;
};
}  // namespace GSAlphabet
#endif  // GROGGSCRIPT_ARRAY_LITERAL_HPP
