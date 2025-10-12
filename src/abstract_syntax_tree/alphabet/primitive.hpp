#pragma once
#include <string>
namespace GSAlphabet {
enum PrimitiveType { STRING, INTEGER, FLOAT, BOOLEAN };
struct Primitive {
    PrimitiveType type;
    union {
        long long int integer;
        std::string text;
        bool boolean;
        double decimal;
    };
    Primitive(const Primitive& other);
    ~Primitive();
    Primitive(long long int val) : integer(val), type(INTEGER) {}
    Primitive(const std::string& val) : text(val), type(STRING) {}
    Primitive(const char* val) : text(val), type(STRING) {}
    Primitive(bool val) : boolean(val), type(BOOLEAN) {}
    Primitive(double val) : decimal(val), type(FLOAT) {}
};

inline Primitive::Primitive(const Primitive& other) : type(other.type) {
    switch (other.type) {
        case INTEGER:
            integer = other.integer;
            break;
        case STRING:
            new (&text)
                std::string(other.text);  // Placement new for std::string
            break;
        case BOOLEAN:
            boolean = other.boolean;
            break;
        case FLOAT:
            decimal = other.decimal;
            break;
    }
}

inline Primitive::~Primitive() {
    if (type == STRING) {
        text.~basic_string();
    }
}
inline std::string to_string(const Primitive& value) {
    switch (value.type) {
        case INTEGER:
            return std::to_string(value.integer);
        case STRING:
            return value.text;
        case BOOLEAN:
            return value.boolean ? "true" : "false";
        case FLOAT:
            return std::to_string(value.decimal);
        default:
            throw std::runtime_error("Primitive type not recognized");
    }
}
}  // namespace GSAlphabet