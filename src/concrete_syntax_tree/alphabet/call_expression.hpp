#pragma once
#include <memory>
#include <vector>

#include "abstract_alphabet_node.hpp"
namespace GSAlphabet {
/// This is an expression like a function call.
/// Composed of a callee, and optionally some args.
/// Examples:
///
/// foo()
///
/// foo("poopypants") // Here the callee is foo
///
/// bar(foo)("pantsypoop")
class CallExpression : public AbstractAlphabetNode {
   public:
    /**
     *
     * @param callee
     */
    explicit CallExpression(std::unique_ptr<AbstractAlphabetNode> callee)
        : callee(std::move(callee)) {}
    /**
     * Creates a call expression with a list of arguments.
     * @param callee Symbol or expression that should be callable
     * @param args Arguments for that function, if this known to be empty,
     * prefer calling the constructor without args parameter
     */
    explicit CallExpression(
        std::unique_ptr<AbstractAlphabetNode> callee,
        std::vector<std::unique_ptr<AbstractAlphabetNode>>&& args)
        : CallExpression(std::move(callee)) {
        arguments = std::move(args);
    }
    std::unique_ptr<AbstractAlphabetNode> callee;
    std::vector<std::unique_ptr<AbstractAlphabetNode>> arguments;
    void accept(AbstractAlphabetNodeVisitor* visitor) override {
        visitor->visitCallExpression(this);
    };
};
}  // namespace GSAlphabet