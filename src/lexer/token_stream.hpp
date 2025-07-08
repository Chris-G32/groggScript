#pragma once
#include <vector>
#include "tokenizer.hpp"
namespace GroggScript
{
    class TokenStream
    {
    public:
        TokenStream(const std::vector<Token> &tokens) : _tokens(tokens), _position(tokens.cbegin())
        {
        }
        /**
         * Peeks count ahead in the stream, without consuming.
         */
        Token peek(uint count) const
        {
            auto peeked = std::next(_position, count);
            if (peeked == _tokens.cend())
            {
                return end();
            }
            return *peeked;
        }
        Token advance(uint count) {}
        const Token end() const { return {TokenType::END_OF_FILE, ""}; }

    protected:
    private:
        const std::__1::vector<GroggScript::Token>::const_iterator _position;
        const std::vector<Token> &_tokens;
    };
}