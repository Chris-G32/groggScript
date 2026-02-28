//
// Created by Christopher Gruska on 1/28/26.
//

#include "tokenizer2.hpp"

namespace GroggScript {
char Tokenizer2::peek() const {
    if (current_ >= source_.size()) return '\0';
    return source_[current_];
}
Token Tokenizer2::nextToken() {
    skipWhitespaceAndComments();
    start_ = current_;

    if (current_ >= source_.size()) {
        return makeToken(TokenType::END_OF_FILE);
    }

    char c = advance();

    // Single-character tokens
    switch (c) {
        case '(':
            return makeToken(TokenType::OPEN_PARENTHESES);

        case ')':
            return makeToken(TokenType::CLOSE_PARENTHESES);
        case '{':
            return makeToken(TokenType::OPEN_CURLY_BRACE);
        case '}':
            return makeToken(TokenType::CLOSE_CURLY_BRACE);
        case '[':
            return makeToken(TokenType::OPEN_BRACKET);
        case ']':
            return makeToken(TokenType::CLOSE_BRACKET);
        case ',':
            return makeToken(TokenType::COMMA);
        case ';':
            return makeToken(TokenType::SEMICOLON);
        case ':':
            return makeToken(TokenType::COLON);
        case '+':
            return makeToken(match('+') ? TokenType::DOUBLE_PLUS : TokenType::PLUS);
        case '-':
            if (match('>')) {
                return makeToken(TokenType::ARROW);
            }
            return makeToken(match('-') ? TokenType::DOUBLE_DASH : TokenType::DASH);
        case '*':
            return makeToken(TokenType::ASTERISK);
        case '/':
            // Comments should not be possible here
            return makeToken(TokenType::FORWARD_SLASH);
        case '!':
            return makeToken(match('=') ? TokenType::EXCLAMATION_EQUALS : TokenType::EXCLAMATION);
        case '=':
            return makeToken(match('=') ? TokenType::DOUBLE_EQUALS : TokenType::EQUALS);
        case '<':
            return makeToken(match('=') ? TokenType::LESS_THAN_EQUALS : TokenType::LESS_THAN);
        case '>':
            return makeToken(match('=') ? TokenType::GREATER_THAN_EQUALS : TokenType::GREATER_THAN);
        case '%':
            return makeToken(TokenType::PERCENT);
        case '.':
            return makeToken(match('.') ? TokenType::DOUBLE_DOT : TokenType::DOT);

        default:
            break;
    }

    if (std::isalpha(c) || c == '_') return process_keywords_and_symbols();
    if (std::isdigit(c)) return number();
    if (c == '"') return string();

    return errorToken("Unexpected character");
}

bool Tokenizer2::validIndex(const size_t index) const { return index < source_.size(); }
char Tokenizer2::advance() {
    const char c = peek();
    current_++;
    column_++;
    return c;
}

bool Tokenizer2::match(const char expected) {
    if (peek() != expected) return false;
    advance();
    return true;
}

void Tokenizer2::skipWhitespaceAndComments() {
    for (;;) {
        switch (const char c = peek()) {
            case ' ':
            case '\t':
            case '\r':
                advance();
                break;

            case '\n':
                advance();
                line_++;
                column_ = 1;
                break;

            case '/': {
                // Make sure we can look ahead safely
                if (!validIndex(current_ + 1)) {
                    return;
                }

                char next = source_[current_ + 1];

                // Line comment //
                if (next == '/') {
                    advance();  // consume '/'
                    advance();  // consume second '/'
                    while (peek() != '\n' && peek() != '\0') {
                        advance();
                    }
                }
                // Block comment /* ... */
                else if (next == '*') {
                    advance();  // consume '/'
                    advance();  // consume '*'
                    while (peek() != '\0') {
                        if (peek() == '*' && validIndex(current_ + 1) &&
                            source_[current_ + 1] == '/') {
                            advance();  // '*'
                            advance();  // '/'
                            break;
                        }
                        advance();
                    }
                } else {
                    return;
                }
                break;
            }

            default:
                return;
        }
    }
}

Token Tokenizer2::makeToken(TokenType type, const std::string& val) const {
    return Token{type, val, line_, column_, current_ - start_};
}
Token Tokenizer2::makeToken(TokenType type) const {
    return makeToken(type, source_.substr(start_, current_ - start_));
}
Token Tokenizer2::errorToken(const std::string& message) const {
    return Token{TokenType::ERROR, message, line_, column_, 0};
}
Token Tokenizer2::process_keywords_and_symbols() {
    while (std::isalnum(peek()) || peek() == '_') advance();

    std::string text = source_.substr(start_, current_ - start_);

    static std::unordered_map<std::string, TokenType> keywords{
        {"if", TokenType::IF_KEYWORD},
        {"for", TokenType::FOR_KEYWORD},
        {"return", TokenType::RETURN_KEYWORD},
        {"bool", TokenType::RESERVED_BOOLEAN_TYPE},
        {"string", TokenType::RESERVED_STRING_TYPE},
        {"int", TokenType::RESERVED_INTEGER_TYPE},
        {"float", TokenType::RESERVED_FLOAT_TYPE},
        {"var", TokenType::RESERVED_VAR_KEYWORD},
        {"fn", TokenType::FUNCTION_MARKER},
        {"false", TokenType::FALSE},
        {"true", TokenType::TRUE}};

    auto it = keywords.find(text);
    if (it != keywords.end()) {
        return makeToken(it->second);
    }

    return makeToken(TokenType::SYMBOL);
}
Token Tokenizer2::number() {
    bool isFloat = false;

    // Integer part
    while (std::isdigit(peek())) {
        advance();
    }

    // Fractional part
    if (peek() == '.' && validIndex(current_ + 1) && std::isdigit(source_[current_ + 1])) {
        isFloat = true;
        advance();  // consume '.'
        while (std::isdigit(peek())) {
            advance();
        }
    }
    return makeToken(isFloat ? TokenType::FLOAT : TokenType::INTEGER);
}

Token Tokenizer2::string() {
    std::string value;

    while (peek() != '"' && peek() != '\0') {
        char c = advance();

        if (c == '\n') {
            line_++;
            column_ = 1;
            value += '\n';  // preserve line break if allowed
        } else if (c == '\\') {
            // handle escape sequence
            if (peek() == '\0') {
                return errorToken("Unterminated escape sequence");
            }

            char esc = advance();
            switch (esc) {
                case 'n':
                    value += '\n';
                    break;
                case 't':
                    value += '\t';
                    break;
                case 'r':
                    value += '\r';
                    break;
                case '"':
                    value += '"';
                    break;
                case '\\':
                    value += '\\';
                    break;
                default:
                    return errorToken("Invalid escape sequence");
            }
        } else {
            value += c;
        }
    }

    if (peek() == '\0') {
        return errorToken("Unterminated string");
    }

    advance();  // consume closing quote

    return makeToken(TokenType::STRING_VALUE, value);
}

}  // namespace GroggScript