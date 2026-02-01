#ifndef GROGGSCRIPT_FOR_LOOP_HPP
#define GROGGSCRIPT_FOR_LOOP_HPP
#include <memory>

#include "statement.hpp"
namespace GSAlphabet {
class ForLoop : public Statement {
   public:
    ForLoop(std::unique_ptr<AbstractAlphabetNode> init,
            std::unique_ptr<AbstractAlphabetNode> condition,
            std::unique_ptr<AbstractAlphabetNode> update,
            std::unique_ptr<AbstractAlphabetNode> loop_body)
        : Statement(std::move(loop_body)),
          init(std::move(init)),
          condition(std::move(condition)),
          update(std::move(update)) {}

    std::unique_ptr<AbstractAlphabetNode> init;
    std::unique_ptr<AbstractAlphabetNode> condition;
    std::unique_ptr<AbstractAlphabetNode> update;

    void accept(AbstractAlphabetNodeVisitor* visitor) override { visitor->visitForLoop(this); }
    [[nodiscard]] SourceLocation location() const override { return init->location(); };
};
}  // namespace GSAlphabet
#endif  // GROGGSCRIPT_FOR_LOOP_HPP
