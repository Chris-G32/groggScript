#include "tokenizer.hpp"

#include <iterator>
#include <regex>
#include <unordered_set>

#include "../logger/logger.hpp"
#define STRING_TYPE_KEYWORD "string"
#define INTEGER_TYPE_KEYWORD "int"
#define BOOLEAN_TYPE_KEYWORD "bool"
#define VAR_TYPE_KEYWORD "var"
#define SIMPLE_HANDLER(ch, tokenType)                       \
    case ch: {                                              \
        _tokens.push_back({tokenType, std::string(1, ch)}); \
        advance();                                          \
        break;                                              \
    }

void GroggScript::Tokenizer::generateTokens() {
    bool keepGoing = true;
    const auto logger = GroggLog::get();
    while (_it != _rawString.end()) {
        auto currentChar = *_it;
        switch (currentChar) {
            case '\0': {
                logger.warn("Null char encountered, this is not expected.");
                logger.debug("Null char at" + getLinePos());
            }
                SIMPLE_HANDLER('[', TokenType::OPEN_BRACKET)
                SIMPLE_HANDLER(']', TokenType::CLOSE_BRACKET)
                SIMPLE_HANDLER('{', TokenType::OPEN_CURLY_BRACE)
                SIMPLE_HANDLER('}', TokenType::CLOSE_CURLY_BRACE)
                SIMPLE_HANDLER('(', TokenType::OPEN_PARENTHESES)
                SIMPLE_HANDLER(')', TokenType::CLOSE_PARENTHESES)
                SIMPLE_HANDLER('.', TokenType::DOT)
                SIMPLE_HANDLER(':', TokenType::COLON)
                SIMPLE_HANDLER(';', TokenType::SEMICOLON)

            case '+': {
                addOneOrDouble('+', TokenType::PLUS, TokenType::DOUBLE_PLUS);
                break;
            }
            case '-': {
                addOneOrDouble('-', TokenType::DASH, TokenType::DOUBLE_DASH);
                break;
            }
            case '=': {
                addOneOrDouble('=', TokenType::EQUALS,
                               TokenType::DOUBLE_EQUALS);
                break;
            }
            case '/': {
                char next = advance();
                if (next == '/') {
                    next = advance();
                    while (next != '\n' && next != '\0') {
                        next = advance();
                    }
                } else if (next == '*') {
                    next = advance();
                    while (next != '\0' && !(next == '*' && peek() == '/')) {
                        next = advance();
                    }
                    advance();
                    advance();
                    if (next == '\0') {
                        logger.warn("Never ending multiline comment");
                    }
                } else {
                    _tokens.push_back({TokenType::FORWARD_SLASH, "/"});
                }
                break;
            }
            case '*':
                _tokens.push_back({TokenType::ASTERISK, "*"});
                advance();
                break;
            case '<': {
                addDependingOnNext('=', TokenType::LESS_THAN,
                                   TokenType::LESS_THAN_EQUALS);
                break;
            }
            case '>': {
                addDependingOnNext('=', TokenType::GREATER_THAN,
                                   TokenType::GREATER_THAN_EQUALS);
                break;
            }
            case '!': {
                addDependingOnNext('=', TokenType::EXCLAMATION,
                                   TokenType::EXCLAMATION_EQUALS);
                break;
            }
            case '"': {
                string builder;  // Start with open quote
                char next = advance();
                bool foundClose = false;
                bool escaped = false;
                while ((next != '\0') && (!foundClose) && (next != '\n')) {
                    if (next == '\\' && !escaped) {
                        escaped = true;
                        auto peeked = peek();
                        if (peeked != '\"' && peeked != '\\') {
                            logger.warn("Unnecessary escape encountered at " +
                                        getLinePos());
                        }
                        next = advance();
                        continue;
                    } else if (next == '\"' && !escaped) {
                        foundClose = true;
                        advance();
                        continue;
                    }
                    escaped = false;
                    builder.append(1, next);
                    next = advance();
                }

                if (!foundClose) {
                    throw std::runtime_error(
                        "Expected a closing quotation mark.");
                }
                _tokens.push_back({TokenType::STRING_VALUE, builder});
                break;
            }
            default: {
                if (isnumber(currentChar)) {
                    logger.debug("Parsing number");
                    string number(1, currentChar);
                    char next = advance();  // advance here instead of peeking
                                            // then advancing.
                    int decimalCount = 0;
                    auto isValid = [&decimalCount](char value) {
                        return isnumber(value) ||
                               (value == '.' && decimalCount < 1);
                    };
                    auto processCharacter = [&number, &next, this]() {
                        number.append(1, next);
                        next = advance();
                    };
                    while (isValid(next) && (next != '\0')) {
                        if (next == '.') {
                            decimalCount += 1;
                        }
                        processCharacter();
                    }
                    const TokenType tokenType = decimalCount == 0
                                                    ? TokenType::INTEGER
                                                    : TokenType::FLOAT;
                    _tokens.push_back({tokenType, number});
                    break;
                } else if (isalpha(currentChar) || currentChar == '_') {
                    logger.debug("Parsing symbol");
                    string working(1, currentChar);
                    auto next = advance();

                    while (isalnum(next) || next == '_') {
                        working.append(1, next);
                        next = advance();
                    }
                    if (working == INTEGER_TYPE_KEYWORD) {
                        _tokens.push_back(
                            {TokenType::RESERVED_INTEGER_TYPE, working});
                        break;
                    } else if (working == STRING_TYPE_KEYWORD) {
                        _tokens.push_back(
                            {TokenType::RESERVED_STRING_TYPE, working});
                        break;
                    } else if (working == BOOLEAN_TYPE_KEYWORD) {
                        _tokens.push_back(
                            {TokenType::RESERVED_BOOLEAN_TYPE, working});
                        break;
                    } else if (working == VAR_TYPE_KEYWORD) {
                        _tokens.push_back(
                            {TokenType::RESERVED_VAR_KEYWORD, working});
                        break;
                    } else if (working == "true") {
                        _tokens.push_back({TokenType::TRUE, working});
                        break;
                    } else if (working == "false") {
                        _tokens.push_back({TokenType::FALSE, working});
                        break;
                    }
                    _tokens.push_back({TokenType::SYMBOL, working});
                    break;
                }
                advance();
            }
        }
    }
    _tokens.push_back({TokenType::END_OF_FILE, ""});
}