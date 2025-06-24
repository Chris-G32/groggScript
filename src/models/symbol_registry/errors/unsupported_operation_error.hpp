#ifndef UNSUPPORTED_OPERATION_ERROR_HPP
#define UNSUPPORTED_OPERATION_ERROR_HPP
#include <stdexcept>
#include <string>

class unsupported_operation_error : public std::runtime_error
{
public:
    explicit unsupported_operation_error(const std::string &message)
        : std::runtime_error(message) {}

    const char *what() const noexcept override
    {
        return std::runtime_error::what();
    }
};
#endif