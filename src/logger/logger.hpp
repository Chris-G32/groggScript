#ifndef GROGGSCRIPT_LOGGER_HPP
#define GROGGSCRIPT_LOGGER_HPP
#include <iostream>
#include <stdexcept>
#include <string>
using std::string;
enum LogLevel { DEBUG, INFO, WARNING, ERROR };
struct LoggerOptions {
    bool debugEnabled = false;
    std::ostream &output = std::cout;
};

class GroggLog {
   public:
    // Options are ignored after the first call
    static const GroggLog &get(LoggerOptions options = {}) {
        static GroggLog log(options.debugEnabled, options.output);
        return log;
    }

    void debug(const string &text) const {
        if (_debugEnabled) {
            log(DEBUG, text);
        }
    }
    void debug(const char *text) const {
        if (_debugEnabled) {
            log(DEBUG, text);
        }
    }
    void info(const string &text) const { log(INFO, text); }
    void info(const char *text) const { log(INFO, text); }
    void warn(const string &text) const { log(WARNING, text); }
    void warn(const char *text) const { log(WARNING, text); }
    void error(const string &text) const { log(ERROR, text); }
    void error(const char *text) const { log(ERROR, text); }

    void log(const LogLevel level, const std::string &text) const {
        constexpr const char *RESET = "\033[0m";
        constexpr const char *RED = "\033[31m";
        constexpr const char *GREEN = "\033[32m";
        constexpr const char *YELLOW = "\033[33m";
        constexpr const char *CYAN = "\033[36m";

        _out << "[";

        switch (level) {
            case DEBUG:
                _out << CYAN << "DEBUG" << RESET;
                break;
            case INFO:
                _out << GREEN << "INFO" << RESET;
                break;
            case WARNING:
                _out << YELLOW << "WARN" << RESET;
                break;
            case ERROR:
                _out << RED << "ERROR" << RESET;
                break;
            default:
                throw std::invalid_argument("Received invalid LogLevel");
        }
        _out << "] " << text << std::endl;
    }

   protected:
    explicit GroggLog(bool debugEnabled = false, std::ostream &output = std::cout)
        : _out(output), _debugEnabled(debugEnabled) {}
    std::ostream &_out;
    bool _debugEnabled;
};
#define DEBUG_LOG(val) GroggLog::get().debug(val);
#define INFO_LOG(val) GroggLog::get().info(val);
#define ERROR_LOG(val) GroggLog::get().error(val);

#endif