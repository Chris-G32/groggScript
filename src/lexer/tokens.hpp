#ifndef GS_TOKENS_HPP
#define GS_TOKENS_HPP

#include <string>
using std::string;

namespace GroggScript {

enum class TokenType {
    // Token value representing null, not intended to store any data, gets
    // around having to use pointer null bs.
    NULL_TOKEN,
    FUNCTION_MARKER,  // fn
    // Mostly for arithmetic
    ASTERISK,
    FORWARD_SLASH,
    DOUBLE_FORWARD_SLASH,
    COMMENT,
    PLUS,
    DOUBLE_DASH,
    DOUBLE_PLUS,
    DASH,
    PERCENT,
    DOT,
    DOUBLE_DOT,
    COLON,
    SEMICOLON,
    END_OF_FILE,
    // Mostly Booleans
    EXCLAMATION,
    EXCLAMATION_EQUALS,
    LESS_THAN,
    GREATER_THAN,
    EQUALS,
    DOUBLE_EQUALS,
    LESS_THAN_EQUALS,
    GREATER_THAN_EQUALS,
    PIPE,
    OR,
    AND,
    // Symbols and stuff
    INTEGER,
    FLOAT,
    WHITE_SPACE,
    STRING_VALUE,
    SYMBOL,
    COMMA,
    OPEN_BRACKET,
    CLOSE_BRACKET,
    OPEN_PARENTHESES,
    CLOSE_PARENTHESES,
    OPEN_CURLY_BRACE,
    CLOSE_CURLY_BRACE,
    RESERVED_INTEGER_TYPE,
    RESERVED_FLOAT_TYPE,
    RESERVED_STRING_TYPE,
    RESERVED_BOOLEAN_TYPE,
    TRUE,
    FALSE,
    RESERVED_VAR_KEYWORD,
    RETURN_KW
};

struct Token {
    TokenType token;
    string value;
};
const Token NULL_TOKEN = {TokenType::NULL_TOKEN, ""};
inline string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::ASTERISK:
            return "asterisk";
        case TokenType::COLON:
            return "colon";
        case TokenType::SEMICOLON:
            return "semicolon";
        case TokenType::FORWARD_SLASH:
            return "forward_slash";
        case TokenType::DOUBLE_FORWARD_SLASH:
            return "double_forward_slash";
        case TokenType::PLUS:
            return "plus";
        case TokenType::DOUBLE_PLUS:
            return "double_plus";
        case TokenType::DOUBLE_DASH:
            return "double_dash";
        case TokenType::DASH:
            return "dash";
        case TokenType::PERCENT:
            return "percent";
        case TokenType::END_OF_FILE:
            return "end_of_file";
        case TokenType::EXCLAMATION:
            return "exclamation";
        case TokenType::EXCLAMATION_EQUALS:
            return "exclamation_equals";
        case TokenType::LESS_THAN:
            return "less_than";
        case TokenType::GREATER_THAN:
            return "greater_than";
        case TokenType::EQUALS:
            return "equals";
        case TokenType::DOUBLE_EQUALS:
            return "double_equals";
        case TokenType::LESS_THAN_EQUALS:
            return "less_than_equals";
        case TokenType::GREATER_THAN_EQUALS:
            return "greater_than_equals";
        case TokenType::PIPE:
            return "pipe";
        case TokenType::OR:
            return "or";
        case TokenType::AND:
            return "and";
        case TokenType::DOT:
            return "dot";
        case TokenType::INTEGER:
            return "integer_literal";
        case TokenType::FLOAT:
            return "float_literal";
        case TokenType::WHITE_SPACE:
            return "white_space";
        case TokenType::RESERVED_INTEGER_TYPE:
            return "int";
        case TokenType::RESERVED_FLOAT_TYPE:
            return "float";
        case TokenType::RESERVED_STRING_TYPE:
            return "string";
        case TokenType::RESERVED_BOOLEAN_TYPE:
            return "bool";
        case TokenType::STRING_VALUE:
            return "string_value";
        case TokenType::SYMBOL:
            return "symbol";
        case TokenType::OPEN_BRACKET:
            return "open_bracket";
        case TokenType::CLOSE_BRACKET:
            return "close_bracket";
        case TokenType::OPEN_CURLY_BRACE:
            return "open_brace";
        case TokenType::CLOSE_CURLY_BRACE:
            return "close_brace";
        case TokenType::COMMENT:
            return "comment";
        case TokenType::TRUE:
            return "true";
        case TokenType::FALSE:
            return "false";
        case TokenType::OPEN_PARENTHESES:
            return "open_parenthese";
        case TokenType::CLOSE_PARENTHESES:
            return "close_parenthese";
        case TokenType::RESERVED_VAR_KEYWORD:
            return "var";
        default:
            return "unknown_token";
    }
}

}  // namespace GroggScript

#endif
