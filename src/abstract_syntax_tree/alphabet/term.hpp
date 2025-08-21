#pragma once
#include "abstract_alphabet_node.hpp"
#include "operator.hpp"
#include "primitive_value.hpp"
#include "symbol.hpp"

namespace GSAlphabet {
enum TermTag { SYMBOL, PRIMITIVE, SINGLE_EXPR };
class Term : public AbstractAlphabetNode {
   public:
    TermTag type;
    union {
        Symbol symbol;
        Primitive primitive;
    };
    Term(Symbol symbol) : symbol(symbol), type(SYMBOL) {}
    Term(Primitive primitive) : primitive(primitive), type(PRIMITIVE) {}
    virtual void accept(AbstractAlphabetNodeVisitor* visitor) override;
};

inline void Term::accept(AbstractAlphabetNodeVisitor* visitor) {
    visitor->visitTerm(this);
}

}  // namespace GSAlphabet
