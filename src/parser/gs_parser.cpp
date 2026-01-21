#include "gs_parser.hpp"

#include "../concrete_syntax_tree/alphabet/binary_expression.hpp"
#include "../concrete_syntax_tree/alphabet/conditional_statement.hpp"
#include "../concrete_syntax_tree/alphabet/for_loop.hpp"
#include "../concrete_syntax_tree/alphabet/function_declaration.hpp"
#include "../concrete_syntax_tree/alphabet/literal.hpp"
#include "../concrete_syntax_tree/alphabet/symbol.hpp"
#include "../concrete_syntax_tree/alphabet/unary_expression.hpp"

using namespace GSAlphabet;
using std::unique_ptr;
unique_ptr<Program> GsParser::program() {
    _current = _tokens.cbegin();
    return std::make_unique<Program>(statements());
}
unique_ptr<Statements> GsParser::statements() {
    std::vector<unique_ptr<AbstractAlphabetNode>> statements;
    statements.reserve(64);
    while (!end()) {
        if (auto stmt = statement(); stmt != nullptr) {
            statements.push_back(std::move(stmt));
            continue;
        }
        break;
    }
    return std::make_unique<Statements>(std::move(statements));
}
unique_ptr<Statement> GsParser::statement() {
    std::unique_ptr<Statement> stmt = nullptr;

    if (accept(TokenType::RETURN_KEYWORD)) {
        stmt = std::make_unique<Statement>(expression(), StatementType::RETURN);
    } else if (accept(TokenType::IF_KEYWORD)) {
        auto cond = expression();
        if (cond == nullptr) {
            throw std::runtime_error("Empty condition in conditional");
        }
        expect(TokenType::OPEN_CURLY_BRACE);
        auto body = statements();
        if (body == nullptr) {
            std::cerr << "If condition with empty body. This is probably a bug "
                         "in your code."
                      << std::endl;
        }
        expect(TokenType::CLOSE_CURLY_BRACE);
        return std::make_unique<ConditionalStatement>(std::move(cond), std::move(body));
    } else if (accept(TokenType::FOR_KEYWORD)) {
        expect(TokenType::OPEN_PARENTHESES);
        std::unique_ptr<Statement> init = nullptr;
        if (!accept(TokenType::SEMICOLON)) {
            init = statement();
        }
        auto cond = expression();
        expect(TokenType::SEMICOLON);
        auto update = expression();
        expect(TokenType::CLOSE_PARENTHESES);
        expect(TokenType::OPEN_CURLY_BRACE);
        auto body = statements();
        if (body == nullptr) {
            std::cerr << "Empty for loop body, this is likely an error" << std::endl;
        }
        expect(TokenType::CLOSE_CURLY_BRACE);
        return std::make_unique<ForLoop>(std::move(init), std::move(cond), std::move(update),
                                         std::move(body));
    } else if (auto varDecl = variableDeclaration(); varDecl != nullptr) {
        stmt = std::make_unique<Statement>(std::move(varDecl));
    } else if (auto varAssign = variableAssignment(); varAssign != nullptr) {
        stmt = std::make_unique<Statement>(std::move(varAssign));
    } else if (auto expr = expression(); expr != nullptr) {
        stmt = std::make_unique<Statement>(std::move(expr));
    } else if (auto funcDecl = functionDeclaration(); funcDecl != nullptr) {
        return std::make_unique<Statement>(std::move(funcDecl));
    } else {
        return nullptr;
    }
    expect(TokenType::SEMICOLON);
    return stmt;
}
std::optional<BinaryOperator> getBinaryOperator(TokenType token) {
    switch (token) {
        case TokenType::OR:
            return std::make_optional<BinaryOperator>(ADDITION);
        case TokenType::AND:
            return std::make_optional<BinaryOperator>(SUBTRACTION);
        case TokenType::DOUBLE_EQUALS:
            return std::make_optional(LOGICAL_EQUALS);
        case TokenType::EXCLAMATION_EQUALS:
            return std::make_optional(LOGICAL_AND);
        case TokenType::LESS_THAN:
            return std::make_optional(LESS_THAN);
        case TokenType::LESS_THAN_EQUALS:
            return std::make_optional(LESS_THAN_EQUALS);
        case TokenType::GREATER_THAN:
            return std::make_optional(GREATER_THAN);
        case TokenType::GREATER_THAN_EQUALS:
            return std::make_optional(GREATER_THAN_EQUALS);
        case TokenType::PLUS:
            return std::make_optional(ADDITION);
        case TokenType::DASH:
            return std::make_optional(SUBTRACTION);
        case TokenType::FORWARD_SLASH:
            return std::make_optional(DIVISION);
        case TokenType::ASTERISK:
            return std::make_optional(MULTIPLY);
        default:
            return std::nullopt;
    }
}
std::optional<UnaryOperator> getUnaryOperator(TokenType token) {
    switch (token) {
        case TokenType::DOUBLE_PLUS:
            return std::make_optional(INCREMENT);
        case TokenType::DOUBLE_DASH:
            return std::make_optional(DECREMENT);
        case TokenType::EXCLAMATION:
            return std::make_optional(LOGICAL_NOT);
        default:
            return std::nullopt;
    }
}

unique_ptr<AbstractAlphabetNode> GsParser::expression() {
    auto primaryExpression = [this]() -> unique_ptr<AbstractAlphabetNode> {
        if (accept(TokenType::OPEN_PARENTHESES)) {
            auto expr = expression();
            if (expr == nullptr) {
                throw std::runtime_error("Expected an expression");
            }
            expect(TokenType::CLOSE_PARENTHESES);
            return std::move(expr);
        }

        return term();
    };

    auto postfixExpr = [this, primaryExpression]() -> unique_ptr<AbstractAlphabetNode> {
        auto expr = primaryExpression();
        if (expr == nullptr) {
            return nullptr;
        }
        if (auto op = getUnaryOperator(_current->token)) {
            expr = std::make_unique<UnaryExpression>(std::move(expr), op.value());
            advance();
        } else if (accept(TokenType::OPEN_PARENTHESES)) {
            std::vector<std::unique_ptr<AbstractAlphabetNode>> args;
            while (auto param = expression()) {
                args.push_back(std::move(param));
                if (accept(TokenType::CLOSE_PARENTHESES)) {
                    break;
                }
                expect(TokenType::COMMA);
            }
            expr = std::make_unique<CallExpression>(std::move(expr), std::move(args));
        }
        return expr;
    };
    /* This will be for negating stuff
     *
         auto unaryExpr = [this,
                          primaryExpression]() ->
       unique_ptr<AbstractAlphabetNode> { auto expr = primaryExpression(); if
       (expr == nullptr) { return nullptr;
            }
            while (auto op = getUnaryOperator(_current->token)) {
                expr =
                    std::make_unique<UnaryExpression>(std::move(expr),
       op.value()); advance();
            }
            return expr;
        };
        */
    auto multiplicativeExpr = [this, postfixExpr]() -> unique_ptr<AbstractAlphabetNode> {
        auto expr = postfixExpr();
        if (expr == nullptr) {
            return nullptr;
        }
        auto op = _current->token;
        // ReSharper disable once CppDFALoopConditionNotUpdated
        while (op == TokenType::ASTERISK || op == TokenType::FORWARD_SLASH) {
            advance();
            if (auto rhs = postfixExpr(); rhs != nullptr) {
                expr = std::make_unique<BinaryExpression>(
                    std::move(expr), getBinaryOperator(op).value(), std::move(rhs));
                op = _current->token;
                continue;
            }
            throw std::runtime_error("Expected an expression after operator: " +
                                     GroggScript::tokenTypeToString(op));
        }
        return expr;
    };
    auto additiveExpr = [this, multiplicativeExpr]() -> unique_ptr<AbstractAlphabetNode> {
        auto expr = multiplicativeExpr();
        if (expr == nullptr) {
            return nullptr;
        }
        auto op = _current->token;
        // ReSharper disable once CppDFALoopConditionNotUpdated
        while (op == TokenType::PLUS || op == TokenType::DASH) {
            advance();
            if (auto rhs = multiplicativeExpr(); rhs != nullptr) {
                expr = std::make_unique<BinaryExpression>(
                    std::move(expr), getBinaryOperator(op).value(), std::move(rhs));
                op = _current->token;
                continue;
            }
            throw std::runtime_error("Expected an expression after operator: " +
                                     GroggScript::tokenTypeToString(op));
        }
        return expr;
    };
    auto comparativeExpr = [this, additiveExpr]() -> unique_ptr<AbstractAlphabetNode> {
        auto expr = additiveExpr();
        if (expr == nullptr) {
            return nullptr;
        }
        auto op = _current->token;
        // ReSharper disable once CppDFALoopConditionNotUpdated
        if (op == TokenType::GREATER_THAN_EQUALS || op == TokenType::GREATER_THAN ||
            op == TokenType::LESS_THAN_EQUALS || op == TokenType::LESS_THAN) {
            advance();
            if (auto rhs = additiveExpr(); rhs != nullptr) {
                expr = std::make_unique<BinaryExpression>(
                    std::move(expr), getBinaryOperator(op).value(), std::move(rhs));
            } else {
                throw std::runtime_error("Expected an expression after operator: " +
                                         GroggScript::tokenTypeToString(op));
            }
        }
        return expr;
    };
    auto equalityExpr = [this, comparativeExpr]() -> unique_ptr<AbstractAlphabetNode> {
        auto expr = comparativeExpr();
        if (expr == nullptr) {
            return nullptr;
        }
        auto op = _current->token;
        // ReSharper disable once CppDFALoopConditionNotUpdated
        while (op == TokenType::EXCLAMATION_EQUALS || op == TokenType::DOUBLE_EQUALS) {
            advance();
            if (auto rhs = comparativeExpr(); rhs != nullptr) {
                expr = std::make_unique<BinaryExpression>(
                    std::move(expr), getBinaryOperator(op).value(), std::move(rhs));

                op = _current->token;
                continue;
            }
            throw std::runtime_error("Expected an expression after operator: " +
                                     GroggScript::tokenTypeToString(op));
        }
        return expr;
    };
    return equalityExpr();
}
// unique_ptr<UnaryExpression> GsParser::unaryExpression() {}
// unique_ptr<BinaryExpression> GsParser::binaryExpression() {
//     auto lhs = term();
//     if (lhs == nullptr) {
//         return nullptr;
//     }
//     std::optional<BinaryOperator> op = getBinaryOperator(_current->token);
//     if (!op) {
//         throw std::runtime_error("Invalid expression.");
//     }
//     auto rhs = term();
//     if (rhs == nullptr) {
//         throw std::runtime_error("Invalid binary expression.");
//     }
//     std::vector<unique_ptr<AbstractAlphabetNode>> expressions(64);
//     expressions.push_back(std::make_unique<BinaryExpression>(lhs, op, rhs));
//     while (auto t = term() != nullptr) {
//         if (auto op = getBinaryOperator(_current->token)) {
//         }
//     }
// }

unique_ptr<AbstractAlphabetNode> GsParser::term() {
    unique_ptr<AbstractAlphabetNode> node = nullptr;
    // Check if symbol first bc less expensive
    if (auto sym = symbol(); sym != nullptr) {
        node = std::move(sym);
    } else if (auto lit = literal(); lit != nullptr) {
        node = std::move(lit);
    } else {
        return nullptr;
    }
    return std::move(node);
}
unique_ptr<Literal> GsParser::literal() {
    const auto& token = _current->token;
    const auto& value = _current->value;
    unique_ptr<Literal> lit;
    switch (token) {
        case TokenType::FLOAT:
            lit = std::make_unique<Literal>(stod(value));
            break;
        case TokenType::INTEGER:
            lit = std::make_unique<Literal>(std::stoll(value));
            break;
        case TokenType::STRING_VALUE:
            lit = std::make_unique<Literal>(value);
            break;
        case TokenType::TRUE:
            lit = std::make_unique<Literal>(true);
            break;
        case TokenType::FALSE:
            lit = std::make_unique<Literal>(false);
            break;
        default:
            return nullptr;
    }
    advance();
    return lit;
}
unique_ptr<Symbol> GsParser::symbol() {
    if (_current->token != TokenType::SYMBOL) {
        return nullptr;
    }
    auto sym = std::make_unique<Symbol>(_current->value);
    advance();
    return sym;
}
std::unique_ptr<GSAlphabet::VariableDeclaration> GsParser::variableDeclaration() {
    if (!accept(TokenType::RESERVED_VAR_KEYWORD)) {
        return nullptr;
    }

    auto identifier = expect(TokenType::SYMBOL).value;
    if (auto typeSpec = typeSpecifier()) {
        auto initializer = accept(TokenType::EQUALS) ? expression() : nullptr;
        return std::make_unique<VariableDeclaration>(identifier, typeSpec, std::move(initializer));
    }
    // If we don't specify a type in declaration, you must initialize it.
    expect(TokenType::EQUALS);
    return std::make_unique<VariableDeclaration>(identifier, expression());
}
std::unique_ptr<GSAlphabet::VariableAssignment> GsParser::variableAssignment() {
    if (auto twoAhead = std::next(_current, 1); twoAhead->token != TokenType::EQUALS) {
        return nullptr;
    }
    auto symbolName = expect(TokenType::SYMBOL);
    expect(TokenType::EQUALS);
    if (auto expr = expression(); expr != nullptr) {
        return std::make_unique<VariableAssignment>(std::move(symbolName.value), std::move(expr));
    }
    throw std::runtime_error("Expected an expression after assignment operator. ");
}
std::unique_ptr<GSAlphabet::FunctionDeclaration> GsParser::functionDeclaration() {
    if (!accept(TokenType::FUNCTION_MARKER)) {
        return nullptr;
    }
    auto functionName = expect(TokenType::SYMBOL).value;
    expect(TokenType::OPEN_PARENTHESES);
    std::vector<FunctionParameter> parameters;
    do {
        auto argName = &_current->value;
        if (!accept(TokenType::SYMBOL)) {
            break;
        }
        if (auto typeSpec = typeSpecifier(); typeSpec != std::nullopt) {
            parameters.push_back({.name = *argName, .type = typeSpec.value()});
            continue;
        }
        throw std::runtime_error(
            "Expected a typeSpecifier in function declaration for "
            "argument" +
            *argName);
    } while (accept(TokenType::COMMA));
    expect(TokenType::CLOSE_PARENTHESES);
    std::optional<std::string> returnType = std::nullopt;
    if (accept(TokenType::DASH)) {
        expect(TokenType::GREATER_THAN);
        // For now we will just take whatever is here instead of expecting a
        // symbol
        returnType = _current->value;
        advance();
    }
    expect(TokenType::OPEN_CURLY_BRACE);
    auto decl =
        std::make_unique<FunctionDeclaration>(functionName, parameters, statements(), returnType);
    expect(TokenType::CLOSE_CURLY_BRACE);
    return decl;
}
std::optional<std::string> GsParser::typeSpecifier() {
    if (accept(TokenType::COLON)) {
        switch (_current->token) {
            case TokenType::RESERVED_BOOLEAN_TYPE:
            case TokenType::RESERVED_INTEGER_TYPE:
            case TokenType::RESERVED_FLOAT_TYPE:
            case TokenType::RESERVED_STRING_TYPE:
            case TokenType::SYMBOL: {
                auto typeName = _current->value;
                advance();
                return typeName;
            }
            default: {
                throw std::runtime_error("Invalid type specifier.");
            }
        }
    }
    return std::nullopt;
}