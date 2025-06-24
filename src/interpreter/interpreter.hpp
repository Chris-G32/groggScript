#include <istream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream> // if using istringstream or similar
enum ExecutionMode
{
    DEBUG,
    PRODUCTION
};
class GroggScriptInterpreter
{
private:
    std::istream &_input;
    std::ostream &_output;
    const ExecutionMode _mode;

public:
    GroggScriptInterpreter(std::istream &inputStream, std::ostream &outputStream = std::cout, ExecutionMode mode = DEBUG) : _input(inputStream), _output(outputStream), _mode(mode)
    {
        std::string initMessage = "Interpretter loading";
        _output << initMessage;
    }
    void interpret()
    {

        std::string line;
        while (std::getline(_input, line))
        {
            /* code */
        }
    }
};