#ifndef ABSTRACT_GRAMMAR_SYMBOL_HPP
#define ABSTRACT_GRAMMAR_SYMBOL_HPP
#include <string>
#include <stdexcept>
using std::string;
class AbstractGrammarSymbol
{
public:
    /// @brief Validates that a given string is valid for a symbol.
    /// @param text Text possibly representing a symbol.
    /// @return A pair containing whether it is a valid symbol, and if it is not, a reason.
    virtual std::pair<bool, string> isValid(const std::string &text) const = 0;
    virtual string symbolType() const = 0;
    virtual ~AbstractGrammarSymbol() = default;

protected:
    /// @brief Constructor for an abstract symbol. Receives a text value, and validates the symbol. Throws when invalid.
    /// @param text Text to build a symbol from.
    AbstractGrammarSymbol(const std::string &text)
    {
        const auto isValidResult = isValid(text);
        if (!isValidResult.first)
        {
            throw new std::domain_error("Invalid value for " + symbolType() + " symbol. Reason: " + isValidResult.second);
        }
        _text = text;
    }

    string _text;
};
#endif