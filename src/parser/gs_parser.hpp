#pragma once
#include "../abstract_syntax_tree/alphabet/abstract_expression.hpp"
#include "../abstract_syntax_tree/alphabet/program.hpp"
#include "../abstract_syntax_tree/alphabet/statement.hpp"
#include "../abstract_syntax_tree/alphabet/statements.hpp"
#include "../abstract_syntax_tree/alphabet/term.hpp"
#include "../abstract_syntax_tree/alphabet/variable_assignment.hpp"
#include "../abstract_syntax_tree/alphabet/variable_declaration.hpp"
#include "../lexer/lexer.h"

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
    std::unique_ptr<GSAlphabet::Term> term();
    std::unique_ptr<GSAlphabet::Literal> literal();
    std::unique_ptr<GSAlphabet::Symbol> symbol();
    // std::unique_ptr<GSAlphabet::UnaryExpression> unaryExpression();
    // std::unique_ptr<GSAlphabet::BinaryExpression> binaryExpression();
    std::unique_ptr<GSAlphabet::VariableDeclaration> variableDeclaration() {
        return nullptr;
    };
    std::unique_ptr<GSAlphabet::VariableAssignment> variableAssignment() {
        return nullptr;
    }

   protected:
    /**
     * Expects a token and advances the stream if encountered, throws otherwise.
     * @param expected Expected token
     */
    void expect(const TokenType expected) {
        if (!accept(expected)) {
            throw std::runtime_error("Unexpected token received.");
        }
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
