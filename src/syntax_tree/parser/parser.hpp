#pragma once
#include "../../logger/logger.hpp"
#include "../grammar/abstract_statement.hpp"
#include "../grammar/program.hpp"
#include "../grammar/utils.hpp"
#include "../grammar/var_declaration.hpp"
#define WRAP_IN_DEBUG_LOG(startMessage, endMessage, body) \
    DEBUG_LOG(startMessage) body;                         \
    DEBUG_LOG(endMessage)
namespace GroggScript {
class Parser {
   public:
    Parser(const std::vector<Token> tokens) : _tokens(tokens) {}
    void generateAST() {
        _position = _tokens.begin();
        DEBUG_LOG(GroggScript::tokenTypeToString(_position->token));
        WRAP_IN_DEBUG_LOG("Parsing began", "Parsing finished", parseProgram());
    }

   protected:
    ProgramNode *parseProgram() {
        return new ProgramNode(*parseManyStatements());
    }
    // Parse multiple statements
    Statements *parseManyStatements() {
        std::vector<AbstractStatement *> statements;
        WRAP_IN_DEBUG_LOG(
            "Start statements", "Done parsing statements",
            while (_position->token != TokenType::END_OF_FILE) {
                WRAP_IN_DEBUG_LOG("Parsing single", "done with single",
                                  statements.push_back(parseSingleStatement()));
            });

        return new Statements(statements);
    }
    // parse an individual statement
    AbstractStatement *parseSingleStatement() {
        DEBUG_LOG("In single statement parse.");
        if (_position->token == TokenType::RESERVED_VAR_KEYWORD) {
            return parseVarDeclaration();
        } else {
            DEBUG_LOG("Failed statement." +
                      tokenTypeToString(_position--->token));
            throw std::runtime_error("VAR DECL ONLY SUPPORTED AST");
        }
    }
    VarDeclaration *parseVarDeclaration() {
        DEBUG_LOG("Parsing var declaration...");
        std::vector<Token> tokens;
        Token symbol;
        Token type;
        DEBUG_LOG("Asserting var...");

        assertAndAdvance(*_position, TokenType::RESERVED_VAR_KEYWORD);
        DEBUG_LOG("Asserting symbol...");
        symbol = assertAndAdvance(*_position, TokenType::SYMBOL);

        if (_position->token != TokenType::COLON) {
            assertAndAdvance(*_position, TokenType::EQUALS);
        } else {
            std::advance(_position, 1);
            type = assertAndAdvance(
                *_position, {TokenType::SYMBOL, TokenType::RESERVED_STRING_TYPE,
                             TokenType::RESERVED_BOOLEAN_TYPE,
                             TokenType::RESERVED_INTEGER_TYPE,
                             TokenType::RESERVED_FLOAT_TYPE});
        }

        if (_position->token != TokenType::EQUALS) {
            DEBUG_LOG("Returning var decl" + type.value + "," + symbol.value);
            DEBUG_LOG("Current token" + tokenTypeToString(_position->token));
            return new VarDeclaration(type, symbol);
        }
        std::advance(_position, 1);
        Token initializer;
        try {
            initializer = assertAndAdvance(
                *_position,
                {TokenType::STRING_VALUE, TokenType::TRUE, TokenType::FALSE,
                 TokenType::INTEGER, TokenType::FLOAT});
        } catch (...) {
            DEBUG_LOG(
                "Encountered unsupported token, floats, expr and symbols to "
                "do" +
                GroggScript::tokenTypeToString(_position->token));
            throw std::runtime_error("UNSUPPORTED TOKEN");
        }

        return new VarInstantiation(type, symbol, initializer);
    }
    bool isTypeName(const Token &token) const {
        switch (token.token) {
            case TokenType::SYMBOL:
            case TokenType::RESERVED_STRING_TYPE:
            case TokenType::RESERVED_BOOLEAN_TYPE:
            case TokenType::RESERVED_INTEGER_TYPE:
            case TokenType::RESERVED_FLOAT_TYPE: {
                assert(token.value.length() > 0);
                DEBUG_LOG(std::string("parsed a type name: ") +
                          GroggScript::tokenTypeToString(token.token))
                return true;
            }
            default:
                return false;
        }
    }
    void parseExpression() {}

   private:
    Token assertAndAdvance(Token token, TokenType expected) {
        DEBUG_LOG("Expected:" + tokenTypeToString(expected));
        DEBUG_LOG("Received:" + tokenTypeToString(token.token));
        assertTokenType(token, expected);
        std::advance(_position, 1);
        return token;
    }
    Token assertAndAdvance(const Token &token,
                           const std::vector<TokenType> &accepted) {
        string result;
        for (const auto &i : accepted) {
            result += tokenTypeToString(i) + ",";
        }
        DEBUG_LOG("Expected:" + result);
        DEBUG_LOG("Received:" + tokenTypeToString(token.token));
        assertTokenType(token, accepted);
        std::advance(_position, 1);
        return token;
    }
    ProgramNode head;
    std::__1::vector<Token>::const_iterator _position;
    const std::vector<Token> _tokens;
};
}  // namespace GroggScript