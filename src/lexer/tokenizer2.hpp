#ifndef GROGGSCRIPT_RULE_TOKENIZER_HPP
#define GROGGSCRIPT_RULE_TOKENIZER_HPP
#include <functional>
#include <istream>
#include <optional>

#include "tokens.hpp"

namespace GroggScript {

class Tokenizer2 {
   public:
    explicit Tokenizer2(const std::string& source) : source_(source) {}
    explicit Tokenizer2(std::string&& source) : source_(std::move(source)) {}
    Token nextToken();

   private:
    [[nodiscard]] bool validIndex(size_t index) const;
    [[nodiscard]] char peek() const;
    char advance();
    bool match(char expected);

    void skipWhitespaceAndComments();
    [[nodiscard]] Token makeToken(TokenType type, const std::string& val) const;
    [[nodiscard]] Token makeToken(TokenType type) const;

    [[nodiscard]] Token errorToken(const std::string& message) const;

    Token process_keywords_and_symbols();
    Token number();
    Token string();

    std::string source_;
    size_t start_ = 0;
    size_t current_ = 0;
    size_t line_ = 1;
    size_t column_ = 1;
};

}  // namespace GroggScript

#endif  // GROGGSCRIPT_RULE_TOKENIZER_HPP
