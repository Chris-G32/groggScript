#include <cstddef>
enum Primitives
{
    /// @brief Positive or negative integer data, basically bigint
    INTEGER,
    /// @brief Float data
    NUMBER,
    /// @brief String and character data
    STRING,
    /// @brief True or false
    BOOLEAN
};

struct PrimitiveData
{
public:
    PrimitiveData(Primitives type) : dataType(type) {}
    Primitives dataType;
    void *data;
    unsigned long memSize;
};
