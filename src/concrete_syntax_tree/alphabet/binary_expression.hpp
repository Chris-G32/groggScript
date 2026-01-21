#pragma once
#include "abstract_alphabet_node.hpp"
#include "symbol.hpp"
namespace GSAlphabet {
enum BinaryOperator {
    LOGICAL_OR,
    LOGICAL_AND,
    LOGICAL_EQUALS,
    NOT_EQUALS,
    LESS_THAN,
    LESS_THAN_EQUALS,
    GREATER_THAN,
    GREATER_THAN_EQUALS,
    ADDITION,
    SUBTRACTION,
    DIVISION,
    MULTIPLY
};
inline std::string toString(const BinaryOperator op) {
    switch (op) {
        case LOGICAL_OR:
            return "LOGICAL_OR";
        case LOGICAL_AND:
            return "LOGICAL_AND";
        case LOGICAL_EQUALS:
            return "LOGICAL_EQUALS";
        case NOT_EQUALS:
            return "NOT_EQUALS";
        case LESS_THAN:
            return "LESS_THAN";
        case LESS_THAN_EQUALS:
            return "LESS_THAN_EQUAL";
        case GREATER_THAN:
            return "GREATER_THAN";
        case GREATER_THAN_EQUALS:
            return "GREATER_THAN_EQUALS";
        case ADDITION:
            return "ADDITION";
        case SUBTRACTION:
            return "SUBTRACTION";
        case DIVISION:
            return "DIVISION";
        case MULTIPLY:
            return "MULTIPLY";
        default:
            return "UNKNOWN_OPERATOR";
    }
}

class BinaryExpression : public AbstractAlphabetNode {
   public:
    BinaryExpression(std::unique_ptr<AbstractAlphabetNode> lhs, const BinaryOperator op,
                     std::unique_ptr<AbstractAlphabetNode> rhs)
        : left(std::move(lhs)), right(std::move(rhs)), op(op) {}

    std::unique_ptr<AbstractAlphabetNode> left;
    std::unique_ptr<AbstractAlphabetNode> right;
    BinaryOperator op;

    void accept(AbstractAlphabetNodeVisitor* visitor) override {
        visitor->visitBinaryExpression(this);
    }
};
}  // namespace GSAlphabet
