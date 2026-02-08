#pragma once
#include <stdexcept>
#include <vector>

#include "../../lexer/tokens.hpp"
using namespace GroggScript;
namespace GroggScript {
namespace Errors {
string buildUnexpectedTokenMessage(TokenType expected, TokenType actual) {
    return "Unexpected Token. Received \"" + tokenTypeToString(expected) +
           "\", expected \"" + tokenTypeToString(actual) + "\"";
}
void throwUnexpectedToken(TokenType expected, TokenType actual) {
    throw std::invalid_argument(buildUnexpectedTokenMessage(expected, actual));
}
void throwUnexpectedToken(Token expected, Token actual) {
    throwUnexpectedToken(expected.token, actual.token);
}
}  // namespace Errors

}  // namespace GroggScript
// void throwUnexpectedToken(Token expected, std::vector<Token> actual)
// {
// }
// void throwUnexpectedToken(Token expected, std::vector<Token> actual)
// {
// }