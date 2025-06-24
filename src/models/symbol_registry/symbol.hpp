#ifndef SYMBOL_HPP
#define SYMBOL_HPP
#include <exception>
#include <stdlib.h>
#include <stdexcept>
#include "errors/unsupported_operation_error.hpp"
enum SymbolType
{
    FUNCTION,
    VARIABLE,
};
class AbstractSymbol
{
protected:
public:
    virtual void execute() { throw unsupported_operation_error(""); }
};
#endif