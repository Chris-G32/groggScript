#include <csignal>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "./lexer/lexer.h"
#include "./logger/logger.hpp"
#include "./parser/gs_parser.hpp"
#include "concrete_syntax_tree/printer_visitor.hpp"
#include "interpreter/interpreter_visitor.hpp"
#include "type_checker/type_checker.hpp"
volatile sig_atomic_t g_signal_status;
typedef std::__1::vector<GroggScript::Token>::const_iterator tokenIterator;

void signal_handler(int signal) { g_signal_status = signal; }

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Please specify a path to a file to tokenize." << std::endl;
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

    GroggScript::Tokenizer2 tokenizer(fileContents);

    std::vector<Token> tokens;
    try {
        for (;;) {
            auto token = tokenizer.nextToken();
            tokens.push_back(token);
            if (token.token == TokenType::END_OF_FILE) {
                break;
            }
        }
        DEBUG_LOG("Tokens generated successfully.");
        DEBUG_LOG("Generating AST...");
        GsParser parser(tokens);
        auto prog = parser.program();
        GsInterpreter::TypeChecker typeChecker;
        typeChecker.visit(prog.get());
        if (!typeChecker.getErrors().empty()) {
            for (const auto &e : typeChecker.getErrors()) {
                ERROR_LOG(e);
            }
        } else {
            INFO_LOG("No Type Errors found.");
        }
        DEBUG_LOG("Interpreter starting...");
        GsInterpreter::InterpreterVisitor interpreter;
        interpreter.visit(prog.get());
        DEBUG_LOG("Interpreter ended...");
    } catch (const std::runtime_error &e) {
        std::cout << "Finished Generating With Errors\n" << e.what() << std::endl;
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "Unexpected error encountered\n";
    }
    std::cout << "Print? (v\\y\\N)\n";
    string verbose;
    std::cin >> verbose;
    bool verboseFlag;
    if (verbose == "Y" || verbose == "y") {
        verboseFlag = false;
    } else if (verbose == "v" || verbose == "V") {
        verboseFlag = true;
    } else {
        return 0;
    }
    for (const auto &token : tokens) {
        auto stringRepr = GroggScript::tokenTypeToString(token.token);
        if (verboseFlag) {
            std::cout << '<' << stringRepr << '>' << token.value;
        }
        std::cout << '<' << stringRepr << "/>" << '\n';
    }
    std::cout << '\n';
    // tokenizer.printTags(std::cout, !(verbose == "y" || verbose == "Y"));
    return 0;
}
