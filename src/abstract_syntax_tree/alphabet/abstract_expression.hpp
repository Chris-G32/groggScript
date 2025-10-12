#pragma once
#include "abstract_alphabet_node.hpp"
namespace GSAlphabet {
// type sugar
class AbstractExpression : public AbstractAlphabetNode {
   public:
    void accept(AbstractAlphabetNodeVisitor* visitor) override = 0;
};
}  // namespace GSAlphabet