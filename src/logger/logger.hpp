#ifndef GROGGSCRIPT_LOGGER
#define GROGGSCRIPT_LOGGER
#include <iostream>
#include <string>
#include <stdexcept>
using std::string;
enum LogLevel
{
    DEBUG,
    INFO,
    WARNING
};
struct LoggerOptions
{
    bool debugEnabled = false;
    std::ostream &output = std::cout;
};

class GroggLog
{
public:
    // Options are ignored after the first call
    static const GroggLog &get(LoggerOptions options = {})
    {
        static GroggLog log(options.debugEnabled, options.output);
        return log;
    }

    void debug(const string &text) const
    {
        if (_debugEnabled)
        {
            log(DEBUG, text);
        }
    }
    void debug(const char *text) const
    {
        if (_debugEnabled)
        {
            log(DEBUG, text);
        }
    }
    void info(const string &text) const
    {
        log(INFO, text);
    }
    void info(const char *text) const
    {
        log(INFO, text);
    }
    void warn(const string &text) const
    {
        log(WARNING, text);
    }
    void warn(const char *text) const
    {
        log(WARNING, text);
    }

    void log(const LogLevel level, const string &text) const
    {

        _out << "[";
        switch (level)
        {
        case DEBUG:
            _out << "DEBUG";
            break;
        case WARNING:
            _out << "WARN";
            break;
        case INFO:
            _out << "INFO";
            break;
        default:
            throw std::invalid_argument("Received invalid LogLevel");
        }
        _out << "] " << text << std::endl;
    }

protected:
    GroggLog(bool debugEnabled = false, std::ostream &output = std::cout) : _out(output), _debugEnabled(debugEnabled)
    {
    }
    std::ostream &_out;
    bool _debugEnabled;
};
#define DEBUG_LOG(val) GroggLog::get().debug(val);
#define INFO_LOG(val) GroggLog::get().info(val);

#endif