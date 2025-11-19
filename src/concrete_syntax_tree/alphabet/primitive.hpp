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
