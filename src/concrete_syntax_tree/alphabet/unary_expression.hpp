#pragma once
#include <utility>
namespace GSAlphabet {
enum UnaryOperator { INVOCATION, INCREMENT, DECREMENT, LOGICAL_NOT };
inline std::string toString(const UnaryOperator op) {
    switch (op) {
        case LOGICAL_NOT:
            return "!";
        case INVOCATION:  // tentative if this is staying
            return "FUNCTION_INVOCATION";
        case INCREMENT:
            return "++";
        case DECREMENT:
            return "--";
        default:
            return "UNKNOWN";
    }
}
class UnaryExpression : public AbstractAlphabetNode {
   public:
    UnaryExpression(std::unique_ptr<AbstractAlphabetNode> node, const UnaryOperator op)
        : node(std::move(node)), op(op) {}
    std::unique_ptr<AbstractAlphabetNode> node;
    UnaryOperator op;
    void accept(AbstractAlphabetNodeVisitor* visitor) override {
        visitor->visitUnaryExpression(this);
    }
    [[nodiscard]] SourceLocation location() const override { return node->location(); }
};
}  // namespace GSAlphabet
