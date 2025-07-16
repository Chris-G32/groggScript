#pragma once
#include <string>

#include "abstract_statement.hpp"
#include "grammar_errors.hpp"
#include "utils.hpp"
using std::string;
/**
var_declaration:
| infer_type symbol type_specifier
| infer_type symbol (type_specifier) assignment expression // Type specifier
must match the primitive or symbols type

 */
typedef string TypeSpecifier;
typedef string SymbolName;

class VarDeclaration : public AbstractStatement {
   protected:
    TypeSpecifier _type;
    SymbolName _identifier;

   public:
    VarDeclaration() {}
    VarDeclaration(Token type, Token identifier) {
        assertTokenIsReservedTypeOrSymbol(type);
        assertTokenType(identifier, TokenType::SYMBOL);
        _type = type.value;
        _identifier = identifier.value;
    }
    virtual string nodeName() override { return "var_declaration"; }
    ~VarDeclaration() {}
};

class VarInstantiation : public VarDeclaration {
   protected:
   public:
    // TODO: make this accept an expression in future instead of symbol or
    // primitib
    VarInstantiation(Token type, Token identifier, Token symbolOrPrimitive)
        : VarDeclaration(type, identifier) {
        assertTokenType(
            symbolOrPrimitive,
            {TokenType::SYMBOL, TokenType::STRING_VALUE, TokenType::INTEGER,
             TokenType::TRUE, TokenType::FALSE, TokenType::FLOAT});
    }
    virtual string nodeName() override { return "var_instantiation"; }
};