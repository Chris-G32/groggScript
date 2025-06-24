#ifndef INTEGER_VALUE_HPP
#define INTEGER_VALUE_HPP
#include <string>
#include <stdexcept>
#include "../../abstract_grammar_symbol.hpp"
using std::string;
class IntegerValue : public AbstractGrammarSymbol
{
public:
    IntegerValue(const std::string &text) : AbstractGrammarSymbol(text) {}
    virtual std::pair<bool, string> isValid(const std::string &text) const override
    {
        if (text.length() < 1)
        {
            return std::make_pair(false, "Too short to be valid integer value.");
        }

        for (const auto &character : text)
        {
            if (!isdigit(character))
            {
                return std::make_pair(false, "Contains a non numeric character");
            }
        }

        return std::make_pair(true, nullptr);
    }
    virtual string symbolType() const override { return "integer_value"; }
};
#endif