#ifndef GS_TOKENS_HPP
#define GS_TOKENS_HPP
#include <string>
using std::string;
namespace GroggScript
{

    enum class TokenType
    {
        // Mostly for arithmetic
        ASTERISK,
        FORWARD_SLASH,
        DOUBLE_FORWARD_SLASH,
        PLUS,
        DOUBLE_PLUS,
        DASH,
        PERCENT,

        END_OF_FILE,
        // Mostly Booleans
        EXCLAMATION,         // Added
        EXCLAMATION_EQUALS,  // Added
        LESS_THAN,           // Added
        GREATER_THAN,        // Added
        EQUALS,              // Added
        DOUBLE_EQUALS,       // Added
        LESS_THAN_EQUALS,    // Added
        GREATER_THAN_EQUALS, // Added
        PIPE,                // this one '|'
        OR,
        AND,
        // Symbols and stuff
        NUMBER,
        WHITE_SPACE,
        RESERVED_TYPE,
        STRING_VALUE,
        SYMBOL,
        OPEN_BRACKET,
        CLOSE_BRACKET,
        OPEN_BRACE,
        CLOSE_BRACE,
    };

    struct Token
    {
        TokenType token;
        string value;
    };

}

#endif