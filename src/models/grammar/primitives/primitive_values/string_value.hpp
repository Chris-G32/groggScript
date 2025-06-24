#ifndef STRING_VALUE_HPP
#define STRING_VALUE_HPP
#include <string>
#include <stdexcept>
#include "../../abstract_grammar_symbol.hpp"
using std::string;
class StringValue : public AbstractGrammarSymbol
{
public:
    /// @brief Takes a string and checks if it is a string variable value. E.g "this" or ""
    /// @param text Text to construct a string value tag from.
    StringValue(const string &text) : AbstractGrammarSymbol(text) {}
    /// @brief Checks if a string is a string value
    /// @param text Text to check if it is a valid string value. I.e starts and ends with ""
    /// @return A pair containing whether the value is a string, and a reason if it is not
    virtual std::pair<bool, string> isValid(const string &text) const override
    {
        if (text.length() < 2)
        {
            return std::make_pair(false, "Too short to be valid string value.");
        }
        if (*text.begin() != '\"')
        {
            return std::make_pair(false, "Expected a \" to begin. ");
        }
        if (*text.end() != '\"')
        {
            return std::make_pair(false, "Expected a \" to end. ");
        }
        return std::make_pair(true, nullptr);
    }
    virtual string symbolType() const override { return "string_value"; }
};
#endif