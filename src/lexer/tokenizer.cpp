#include "tokenizer.hpp"
#include "../logger/logger.hpp"
#include <iterator>
#include <unordered_set>
#include <regex>
#define STRING_TYPE_KEYWORD "string"
#define INTEGER_TYPE_KEYWORD "int"
void GroggScript::Tokenizer::generateTokens()
{
    bool keepGoing = true;
    const auto logger = GroggLog::get();
    while (_it != _rawString.end())
    {
        auto currentChar = *_it;
        switch (currentChar)
        {
        case '+':
        {
            addOneOrDouble('+', TokenType::PLUS, TokenType::DOUBLE_PLUS);
            break;
        }
        case '=':
        {
            addOneOrDouble('=', TokenType::EQUALS, TokenType::DOUBLE_EQUALS);
            break;
        }
        case '/':
        {
            addOneOrDouble('=', TokenType::FORWARD_SLASH, TokenType::DOUBLE_FORWARD_SLASH);
            break;
        }
        case '*':
            _tokens.push_back({TokenType::ASTERISK, "*"});
            break;
        case '<':
        {
            addDependingOnNext('=', TokenType::LESS_THAN, TokenType::LESS_THAN_EQUALS);
            break;
        }
        case '>':
        {
            addDependingOnNext('=', TokenType::GREATER_THAN, TokenType::GREATER_THAN_EQUALS);
            break;
        }
        case '!':
        {
            addDependingOnNext('=', TokenType::EXCLAMATION, TokenType::EXCLAMATION_EQUALS);
            break;
        }
        case '"':
        {
            string builder("\""); // Start with open quote
            char peeked = peek();
            bool foundClose = false;
            while ((peeked != '\0') && (!foundClose) && (peeked != '\n'))
            {
                if (peeked == '\"')
                {
                    foundClose = true;
                }
                builder.append(1, advance());
                peeked = peek();
            }

            if (!foundClose)
            {
                throw std::runtime_error("Expected a closing parentheses.");
            }
            _tokens.push_back({TokenType::STRING_VALUE, builder});
            advance();
            break;
        }
        case '\0':
            logger.debug("Encountered end of string");
            return;
        default:
        {
            if (isnumber(currentChar))
            {
                logger.debug("Parsing number");
                string number(1, currentChar);
                char peeked = advance(); // advance here instead of peeking then advancing.
                int decimalCount = 0;
                auto isValid = [&decimalCount](char value)
                {
                    return isnumber(value) || (value == '.' && decimalCount < 1);
                };
                auto processCharacter = [&number, &peeked, this]()
                {
                    number.append(1, peeked);
                    advance();
                    peeked = peek();
                };
                while (isValid(peeked) && (peeked != '\0'))
                {
                    if (peeked == '.')
                    {
                        decimalCount += 1;
                    }
                    processCharacter();
                }
                _tokens.push_back({TokenType::NUMBER, number});
                advance();
                break;
            }
            else if (isalpha(currentChar) || currentChar == '_')
            {
                logger.debug("Parsing symbol");

                string working(1, currentChar);
                auto next = advance();

                while (isalnum(next) || next == '_')
                {
                    working.append(1, next);
                    next = advance();
                }
                if (working == STRING_TYPE_KEYWORD || working == INTEGER_TYPE_KEYWORD)
                {
                    _tokens.push_back({TokenType::RESERVED_TYPE, working});
                    break;
                }
                _tokens.push_back({TokenType::SYMBOL, working});
                break;
            }
            advance();
        }
        }
    }
}