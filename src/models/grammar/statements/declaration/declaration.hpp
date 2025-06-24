#ifndef DECLARATION_HPP
#define DECLARATION_HPP
#include <string>
#include <stdexcept>
#include <vector>
#include "../../abstract_grammar_symbol.hpp"
#include "primitive_type.hpp"
using PrimitiveTypes::PrimitiveTypeSymbol;
using std::string;
class Declaration : public AbstractGrammarSymbol
{
public:
    Declaration(const string &text) : AbstractGrammarSymbol(text) {}
    ~Declaration()
    {
        for (const auto &item : _symbols)
        {
            delete item;
        }
    }
    /// @brief Validates that a given string is valid for a symbol.
    /// @param text Text possibly representing a symbol.
    /// @return A pair containing whether it is a valid symbol, and if it is not, a reason.
    virtual std::pair<bool, string> isValid(const std::string &text) const
    {
        std::vector<AbstractGrammarSymbol *> symbols;
        auto prevSpaceIndex = 0;
        auto currentSpaceIndex = text.find(' ');
        const auto nextSubstr = [&prevSpaceIndex, &text]()
        {
            auto afterLastSpaceIndex = prevSpaceIndex + 1;
            auto currentSpaceIndex = text.find(' ', afterLastSpaceIndex);
            auto substr = text.substr(afterLastSpaceIndex, currentSpaceIndex - afterLastSpaceIndex);
            prevSpaceIndex = currentSpaceIndex;
            return substr;
        };

        symbols.push_back(new PrimitiveTypeSymbol(nextSubstr()));
        symbols.push_back(new )
    }
    virtual string symbolType() const { return "declaration"; }
};

#endif