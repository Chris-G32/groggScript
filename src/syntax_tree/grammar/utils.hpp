#pragma once
#include <assert.h>

#include <vector>

#include "../../lexer/tokens.hpp"
using namespace GroggScript;
void assertTokenType(Token token, TokenType expected) {
    assert(token.token == expected);
}

void assertTokenType(Token token, const std::vector<TokenType> &accepted) {
    bool isOneOfAccepted = false;
    for (const auto &item : accepted) {
        if (item == token.token) {
            isOneOfAccepted = true;
            break;
        }
    }
    assert(isOneOfAccepted);
}
void assertTokenIsReservedTypeOrSymbol(Token token) {
    assertTokenType(token, {TokenType::SYMBOL, TokenType::RESERVED_STRING_TYPE,
                            TokenType::RESERVED_BOOLEAN_TYPE,
                            TokenType::RESERVED_INTEGER_TYPE,
                            TokenType::RESERVED_FLOAT_TYPE});
}