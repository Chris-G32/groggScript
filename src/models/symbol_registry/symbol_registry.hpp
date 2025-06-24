#ifndef SYMBOL_REGISTRY_HPP
#define SYMBOL_REGISTRY_HPP
#include <map>

class AbstractSymbolRegistry
{
protected:
public:
    virtual void registerSymbol();
};

class SymbolRegistry
{
};
#endif