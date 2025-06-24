#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <signal.h>
#include <stdlib.h>
#include <sstream>
#include <exception>
#include <fstream>
#include <filesystem>
#include <vector>
#include "./lexer/tokenizer.hpp"
#include "./logger/logger.hpp"
volatile sig_atomic_t g_signal_status;

void signal_handler(int signal)
{
    g_signal_status = signal;
}
struct VariableDeclaration
{
    std::string typeSpecifier;
    void *value;
    unsigned long memSize;
};
std::string trim(const std::string &str)
{
    auto start = std::find_if_not(str.begin(), str.end(), ::isspace);
    auto end = std::find_if_not(str.rbegin(), str.rend(), ::isspace).base();

    return (start < end) ? std::string(start, end) : std::string();
};
std::vector<std::string> splitOnWhitespace(const std::string &input)
{
    std::istringstream stream(input);
    std::vector<std::string> tokens;
    std::string token;

    while (stream >> token)
    {
        tokens.push_back(token);
    }

    return tokens;
}
VariableDeclaration processVarDecl(const std::vector<std::string> &line)
{
    VariableDeclaration decl;
    if (line.size() != 4)
    {
        throw std::invalid_argument("Line is not a variable declaration.");
    }
    const auto typeSpecifier = line[0];
    const auto symbolName = line[1];
    const auto value = line[3];
    if (typeSpecifier == "string")
    {
        decl.value = new const char *(value.c_str());
        decl.memSize = value.size();
    }
    else if (typeSpecifier == "int")
    {
        decl.value = new int(std::stoi(value));
        decl.memSize = sizeof(int);
    }
    else if (typeSpecifier == "float")
    {
        decl.value = new float(std::stof(value));
        decl.memSize = sizeof(float);
    }
    else if (typeSpecifier == "bool")
    {
        if (value == "true")
        {
            decl.value = new bool(true);
            decl.memSize = sizeof(bool);
        }
        else if (value == "false")
        {
            decl.value = new bool(false);
            decl.memSize = sizeof(bool);
        }
        else
        {
            throw std::runtime_error("Expected 'true' or 'false', received" + value);
        }
    }
    else
    {
        throw std::invalid_argument("Invalid type specifier");
    }
    return decl;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
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
    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << filePathArg << std::endl;
        logger.warn("Failed to open file: " + filePathArg);
        return 1;
    }

    logger.info("File opened successfully: " + filePathArg);

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    std::string fileContents = buffer.str();

    if (g_signal_status == SIGINT)
    {
        std::cerr << "Process interrupted by SIGINT." << std::endl;
        return 1;
    }

    GroggScript::Tokenizer tokenizer(fileContents);

    try
    {
        tokenizer.generateTokens();
        logger.info("Tokens generated successfully.");
    }
    catch (const std::runtime_error &e)
    {
        logger.warn("Error in token parsing: " + std::string(e.what()));
        std::cout << "Finished Generating With Errors\n"
                  << e.what() << std::endl;
    }

    tokenizer.debugPrint();

    return 0;
}
