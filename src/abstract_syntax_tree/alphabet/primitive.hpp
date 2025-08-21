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

Primitive::Primitive(const Primitive& other) : type(other.type) {
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

Primitive::~Primitive() {
    if (type == STRING) {
        text.~basic_string();
    }
}
}  // namespace GSAlphabet