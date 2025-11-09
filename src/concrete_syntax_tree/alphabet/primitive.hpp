#pragma once
#include <stdexcept>
#include <string>
#include <variant>

namespace GSAlphabet {
typedef long long int integer;
typedef double decimal;
typedef bool boolean;
typedef std::string text;
typedef std::variant<integer, decimal, boolean, text> Primitive;
// enum PrimitiveType { STRING, INTEGER, FLOAT, BOOLEAN };
//
// struct Primitive {
//     PrimitiveType type;
//
//     union {
//         long long int integer;
//         std::string text;
//         bool boolean;
//         double decimal;
//     };
//
//     // Constructors
//     Primitive(long long int val) : integer(val), type(INTEGER) {}
//     Primitive(const std::string& val) : text(val), type(STRING) {}
//     Primitive(const char* val) : text(val), type(STRING) {}
//     Primitive(bool val) : boolean(val), type(BOOLEAN) {}
//     Primitive(double val) : decimal(val), type(FLOAT) {}
//
//     // Copy constructor
//     Primitive(const Primitive& other);
//
//     // Destructor
//     ~Primitive();
//
//     // Copy-swap assignment operator
//     Primitive& operator=(Primitive other);  // Pass-by-value copy
//
//     // Swap function (friend)
//     friend void swap(Primitive& first, Primitive& second) noexcept;
// };
//
// // Implementation of copy constructor
// inline Primitive::Primitive(const Primitive& other) : type(other.type) {
//     switch (other.type) {
//         case INTEGER:
//             integer = other.integer;
//             break;
//         case STRING:
//             new (&text) std::string(other.text);  // placement new
//             break;
//         case BOOLEAN:
//             boolean = other.boolean;
//             break;
//         case FLOAT:
//             decimal = other.decimal;
//             break;
//     }
// }
//
// // Destructor
// inline Primitive::~Primitive() {
//     if (type == STRING) {
//         text.~basic_string();
//     }
// }
//
// // Copy-swap assignment operator
// inline Primitive& Primitive::operator=(Primitive other) {
//     swap(*this, other);
//     return *this;
// }
//
// // This is Chatgpt bullshit that doesnt work and bombs my stack >:[
// inline void swap(Primitive& first, Primitive& second) noexcept {
//     // If both are strings, swap them directly
//     if (first.type == STRING && second.type == STRING) {
//         swap(first.text, second.text);
//     } else {
//         // First, destroy both if they are strings to prevent leaks
//         if (first.type == STRING) {
//             first.text.~basic_string();
//         }
//         if (second.type == STRING) {
//             second.text.~basic_string();
//         }
//         std::swap(first, second);
//     }
//
//     // Finally, swap the type
//     swap(first.type, second.type);
// }

// to_string helper
inline std::string to_string(const Primitive& value) {
    return std::visit(
        [](const auto& val) -> std::string {
            using T = std::decay_t<decltype(val)>;
            if constexpr (std::is_same_v<T, decimal>) {
                return std::to_string(val);
            }
            if constexpr (std::is_same_v<T, integer>) {
                return std::to_string(val);
            }
            if constexpr (std::is_same_v<T, boolean>) {
                return val ? "true" : "false";
            }
            if constexpr (std::is_same_v<T, text>) {
                return val;
            }
            throw std::runtime_error("Invalid primitive value");
        },
        value);
}

}  // namespace GSAlphabet
