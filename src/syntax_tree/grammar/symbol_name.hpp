#include <string>
#include <regex>
class SymbolName
{
    static bool isValidSymbolName(const std::string &string);
};
bool SymbolName::isValidSymbolName(const std::string &string)
{
    if (string.length() == 0)
    {
        return false;
    }
    if (string.at(0) == '_')
    {
        std::regex pattern(R"(^[A-Za-z0-9]+$)");
    }
    else
    {
    }
}