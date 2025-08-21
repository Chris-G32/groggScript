#pragma once
#include "abstract_alphabet_node.hpp"
#include "operator.hpp"
#include "primitive_value.hpp"
#include "symbol.hpp"
#include "term.hpp"
namespace GSAlphabet {
enum 
class Expression : public AbstractAlphabetNode {
   public:
    Expression(Term term) {}
    Expression(Term lhs, Operator op, Term rhs) {}
    virtual void accept(AbstractAlphabetNodeVisitor* visitor) override;
    
};

inline void Expression::accept(AbstractAlphabetNodeVisitor* visitor) {
    visitor->visitExpression(this);
}

}  // namespace GSAlphabet
