#pragma once
#include "../concrete_syntax_tree/alphabet/array_literal.hpp"
#include "../concrete_syntax_tree/alphabet/call_expression.hpp"
#include "../concrete_syntax_tree/alphabet/program.hpp"
#include "../concrete_syntax_tree/alphabet/statement.hpp"
#include "../concrete_syntax_tree/alphabet/statements.hpp"
#include "../concrete_syntax_tree/alphabet/variable_assignment.hpp"
#include "../concrete_syntax_tree/alphabet/variable_declaration.hpp"
#include "../lexer/lexer.h"
using GroggScript::Token;
using GroggScript::TokenType;
class GsParser {
   public:
    explicit GsParser(const std::vector<Token>& tokens)
        : _tokens(tokens), _current(tokens.cbegin()) {}
    explicit GsParser(std::vector<Token>&& tokens)
        : _tokens(std::move(tokens)), _current(_tokens.cbegin()) {}
    std::unique_ptr<GSAlphabet::Program> program();
    std::unique_ptr<GSAlphabet::Statements> statements();
    std::unique_ptr<GSAlphabet::Statement> statement();
    std::unique_ptr<GSAlphabet::AbstractAlphabetNode> expression();
    std::unique_ptr<GSAlphabet::ArrayLiteral> arrayLiteral();
    std::unique_ptr<GSAlphabet::AbstractAlphabetNode> term();
    std::unique_ptr<GSAlphabet::Literal> literal();
    std::unique_ptr<GSAlphabet::Symbol> symbol();
    std::unique_ptr<GSAlphabet::VariableDeclaration> variableDeclaration();
    std::unique_ptr<GSAlphabet::VariableAssignment> variableAssignment();
    std::unique_ptr<GSAlphabet::FunctionDeclaration> functionDeclaration();
    std::optional<std::string> typeSpecifier();

   protected:
    static SourceLocation locationOf(const Token& token) {
        return {.line = token.line, .column = token.column};
    }
    /**
     * Expects a token and advances the stream if encountered, throws otherwise.
     * @param expected Expected token
     */
    auto expect(const TokenType expected) {
        const auto tmp = *_current;
        if (!accept(expected)) {
            throw std::runtime_error("Unexpected token received. Expected " +
                                     tokenTypeToString(expected) + " saw " +
                                     GroggScript::tokenTypeToString(tmp.token));
        }
        return tmp;
    }

    /**
     * Advances stream when we encounter the accepted character.
     * @param accepted Accepted token type.
     * @return True when accepted matches.
     */
    bool accept(TokenType accepted) {
        if (_current->token == accepted) {
            advance();
            return true;
        }
        return false;
    }
    void advance() { std::advance(_current, 1); }
    bool end() {
        if (_current == _tokens.end()) {
            return true;
        }
        return _current->token == TokenType::END_OF_FILE;
    }
    std::vector<Token> _tokens;
    std::vector<Token>::const_iterator _current;
};
