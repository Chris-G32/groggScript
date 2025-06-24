#ifndef GROGGSCRIPT_SYMBOL_HPP
#define GROGGSCRIPT_SYMBOL_HPP
#include "../abstract_grammar_symbol.hpp"
#include <regex>
class Symbol : AbstractGrammarSymbol
{
public:
    Symbol(const std::string &text) : AbstractGrammarSymbol(text) {}
    std::pair<bool, string> isValid(const std::string &text) const override
    {
        static const std::regex isValidVariableName("^(_[a-zA-Z][a-zA-Z0-9_]*|[a-zA-Z][a-zA-Z0-9_]*)$");
        if (!std::regex_match(text, isValidVariableName))
        {
            static const string errorString =
                "Symbols must start with a number or an underscore."
                "When an underscore leads, there must follow at least one alphabetical character.";
            return std::make_pair(false, errorString);
        }
        return std::make_pair(false, nullptr);
    }
    virtual string symbolType() override { return "Symbol"; }
};
Symbol a("text");
#endif