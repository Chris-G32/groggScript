#ifndef PRIMITIVE_TYPE_HPP
#define PRIMITIVE_TYPE_HPP
#include <string>
#include <vector>
#include <stdexcept>
#include <exception>
#include "../abstract_grammar_symbol.hpp"
using std::string;
bool isOneOf(const std::string &value, std::vector<string> values)
{
    for (const auto &val : values)
    {
        if (value == val)
        {
            return true;
        }
    }
    return false;
};

namespace PrimitiveTypes
{
    static const string STRING_KEYWORD = "string";
    static const string INTEGER_KEYWORD = "int";
    static const string BOOLEAN_KEYWORD = "bool";
    static const std::vector<string> PRIMITIVE_KEYWORDS = {STRING_KEYWORD, INTEGER_KEYWORD, BOOLEAN_KEYWORD};
    class PrimitiveTypeSymbol : public AbstractGrammarSymbol
    {
    public:
        PrimitiveTypeSymbol(const string &text) : AbstractGrammarSymbol(text) {}
        virtual std::pair<bool, string> isValid(const std::string &text) const override
        {
            if (!isOneOf(text, PRIMITIVE_KEYWORDS))
            {
                return std::make_pair(false, "Received an invalid type.");
            }
            return std::make_pair(true, nullptr);
        }
        virtual string symbolType() const override { return "primitive_type"; }
        virtual AbstractGrammarSymbol processNext() {}
    };

}
#endif