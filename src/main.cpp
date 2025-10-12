#include <signal.h>
#include <stdlib.h>

#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "./lexer/tokenizer.hpp"
#include "./logger/logger.hpp"
#include "./parser/gs_parser.hpp"
#include "abstract_syntax_tree/printer_visitor.hpp"
volatile sig_atomic_t g_signal_status;
typedef std::__1::vector<GroggScript::Token>::const_iterator tokenIterator;

void signal_handler(int signal) { g_signal_status = signal; }
tokenIterator parseTypeName(const tokenIterator &it) {
    switch (it->token) {
        case TokenType::RESERVED_STRING_TYPE:
        case TokenType::RESERVED_BOOLEAN_TYPE:
        case TokenType::RESERVED_INTEGER_TYPE: {
            DEBUG_LOG(std::string("parsed a type name: ") +
                      GroggScript::tokenTypeToString(it->token))
            return std::next(it);
            break;
        }
        default:
            return it;
    }
}

tokenIterator parseTypeSpecifier(const tokenIterator &it) {
    auto localIt = it;
    if (localIt->token != TokenType::COLON) {
        DEBUG_LOG("NOT a type specifier");
        return localIt;
    }
    std::advance(localIt, 1);
    auto res = parseTypeName(localIt);
    if (localIt == res) {
        throw std::runtime_error("EXPECTED A TYPE NAME");
    }
    return res;
}

tokenIterator parseVarDeclaration(const tokenIterator &it) {
    auto localIt = it;
    DEBUG_LOG("Parsing var declaration...");
    std::vector<Token> tokens;
    if (localIt->token == TokenType::RESERVED_VAR_KEYWORD) {
        DEBUG_LOG("VAR KEYWORD MATCHED...");
        tokens.push_back(*localIt);
    } else {
        throw std::runtime_error("EXPECTED VAR KW");
    }
    std::advance(localIt, 1);
    if (localIt->token == TokenType::SYMBOL) {
        DEBUG_LOG("SYMBOL TOKEN MATCHED...");
        tokens.push_back(*localIt);

        std::advance(localIt, 1);
    } else {
        throw std::runtime_error("INVALID SYNTAX FOR VAR DECLARATION");
    }
    bool typeSpecified = false;
    if (localIt->token == TokenType::COLON) {
        typeSpecified = true;
        DEBUG_LOG("TYPE_SPECIFIED")
        auto res = parseTypeSpecifier(localIt);
        while (localIt != res) {
            tokens.push_back(*localIt);
            std::advance(localIt, 1);
        }
    }
    if (localIt->token != TokenType::EQUALS && !typeSpecified) {
        throw std::runtime_error("MUSt PROVIDE TYPE INFO");
    }
    tokens.push_back(*localIt);
    std::advance(localIt, 1);
    if (localIt->token == TokenType::STRING_VALUE ||
        localIt->token == TokenType::TRUE ||
        localIt->token == TokenType::FALSE ||
        localIt->token != TokenType::INTEGER) {
        tokens.push_back(*localIt);
        std::advance(localIt, 1);
    } else {
        DEBUG_LOG(
            "Encountered unsupported token, floats, expr and symbols to do" +
            GroggScript::tokenTypeToString(localIt->token))
        throw std::runtime_error("ERR");
    }
    DEBUG_LOG("PARSED VAR DECLARATION")
    for (const auto a : tokens) {
        DEBUG_LOG(std::string("<") + GroggScript::tokenTypeToString(a.token) +
                  std::string(">"));
        DEBUG_LOG(a.value);
    }
    return localIt;
}
tokenIterator parseStatement(const tokenIterator &it) {
    DEBUG_LOG("Parsing statement...");
    std::vector<string> statements;
    if (it->token == TokenType::RESERVED_VAR_KEYWORD) {
        DEBUG_LOG("Lookahead is potentially var decl.");
        return parseVarDeclaration(it);
    } else {
        throw std::runtime_error("VAR DECL ONLY SUPPORTED AST");
    }
    return std::next(it);
}
void parseProgram(tokenIterator it) {
    std::vector<string> statements;
    while (it->token != TokenType::END_OF_FILE) {
        try {
            DEBUG_LOG(GroggScript::tokenTypeToString(it->token));
            it = parseStatement(it);
        } catch (std::runtime_error &e) {
            DEBUG_LOG(GroggScript::tokenTypeToString(it->token));
            DEBUG_LOG("Advancing after failure to next token.")
            std::advance(it, 1);
        }
    }
}
void generateAST(const std::vector<Token> &tokens) {
    auto val = tokens.cbegin();
    parseProgram(val);
}
int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Please specify a path to a file to tokenize."
                  << std::endl;
        return 1;
    }

    bool isDebugMode = (argc > 2 && strcmp(argv[2], "-d") == 0);

    LoggerOptions options;
    options.debugEnabled = isDebugMode;
    auto logger = GroggLog::get(options);
    logger.debug("Logger is in debug mode.");

    // Handle CTRL+C
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = signal_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);

    std::string filePathArg(argv[1]);
    std::filesystem::path path(filePathArg);
    std::ifstream file(path.string());
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePathArg << std::endl;
        logger.warn("Failed to open file: " + filePathArg);
        return 1;
    }

    logger.info("File opened successfully: " + filePathArg);

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    std::string fileContents = buffer.str();

    if (g_signal_status == SIGINT) {
        std::cerr << "Process interrupted by SIGINT." << std::endl;
        return 1;
    }

    GroggScript::Tokenizer tokenizer(fileContents);

    try {
        tokenizer.generateTokens();
        logger.info("Tokens generated successfully.");
        logger.info("Generating AST...");
        GsParser parser(tokenizer.getTokens());
        auto prog = parser.program();
        PrinterVisitor vis;
        vis.visit(prog.get());
    } catch (const std::runtime_error &e) {
        logger.warn("Error in token parsing: " + std::string(e.what()) + "at " +
                    tokenizer.getLinePos());
        std::cout << "Finished Generating With Errors\n"
                  << e.what() << std::endl;
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "Unexpected error encountered\n";
    }
    std::cout << "Print verbose? (y\\N)\n";
    string verbose;
    std::cin >> verbose;
    tokenizer.printTags(std::cout, !(verbose == "y" || verbose == "Y"));
    return 0;
}
