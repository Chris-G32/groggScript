#include <sstream>
class Symbol
{
public:
    virtual Symbol processNext(std::stringstream &stream) = 0;
};
class Identifier : Symbol
{
};
class TypeSymbol : public Symbol
{
public:
    Symbol *parent = nullptr;
    Symbol processNext(std::stringstream &stream) override
    {
        // Expect an identifier when
    }
};
class Statement : Symbol
{
};

