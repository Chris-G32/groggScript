//
// Created by Christopher Gruska on 11/16/25.
//

#ifndef GROGGSCRIPT_CONDITIONAL_STATEMENT_HPP
#define GROGGSCRIPT_CONDITIONAL_STATEMENT_HPP
#include "statement.hpp"
namespace GSAlphabet {

class ConditionalStatement : public Statement {
   public:
    ConditionalStatement(std::unique_ptr<AbstractAlphabetNode> condition,
                         std::unique_ptr<AbstractAlphabetNode> statement)
        : Statement(std::move(statement), StatementType::ONE_LINE),
          condition(std::move(condition)) {}
    void accept(AbstractAlphabetNodeVisitor *visitor) override {
        visitor->visitConditionalStatement(this);
    }
    std::unique_ptr<AbstractAlphabetNode> condition;
};
}  // namespace GSAlphabet
#endif  // GROGGSCRIPT_CONDITIONAL_STATEMENT_HPP
