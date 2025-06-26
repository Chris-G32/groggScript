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
        logger.warn("Error in token parsing: " + std::string(e.what()) + "at " + tokenizer.getLinePos());
        std::cout << "Finished Generating With Errors\n"
                  << e.what() << std::endl;
    }
    std::cout << "Print verbose? (y\\N)\n";
    string verbose;
    std::cin >> verbose;
    tokenizer.printTags(std::cout, !(verbose == "y" || verbose == "Y"));

    return 0;
}
