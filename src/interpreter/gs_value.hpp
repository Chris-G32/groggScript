#ifndef GS_VALUE_HPP_
#define GS_VALUE_HPP_
#include <unordered_map>
#include <variant>

#include "../concrete_syntax_tree/alphabet/primitive.hpp"

namespace GsInterpreter {

enum class gs_value_type {
    BOOLEAN,
    FLOAT,
    INTEGER,
    STRING,
    OBJECT,
    FUNCTION,
    EMPTY
};

std::string to_string(gs_value_type value);

using gs_int = long long;
using gs_float = double;
using gs_boolean = bool;
using gs_string = std::string;
using gs_function = class AbstractGsFunction*;
using gs_object = std::unordered_map<std::string, class gs_value>;
using gs_value_variant = std::variant<gs_int, gs_float, gs_boolean, gs_string,
                                      gs_function, gs_object>;
struct gs_value {  // TODO: Switch to using unique ptr, or maybe the object
                   // itself if possible. There is a mem leak as is.
    explicit gs_value(gs_value_variant val);
    gs_value() {
        type = gs_value_type::EMPTY;
        value = false;
    }
    gs_value_type type;
    gs_value_variant value;
    [[nodiscard]] gs_value operator+(const gs_value& rhs) const;
    [[nodiscard]] gs_value operator-(const gs_value& rhs) const;
    [[nodiscard]] gs_value operator*(const gs_value& rhs) const;
    [[nodiscard]] gs_value operator/(const gs_value& rhs) const;
    [[nodiscard]] gs_value operator==(const gs_value& rhs) const;
    gs_value operator++();
    gs_value operator--();
    [[nodiscard]] gs_value operator<(const gs_value& rhs) const;
    [[nodiscard]] gs_value operator<=(const gs_value& rhs) const;
    [[nodiscard]] gs_value operator>(const gs_value& rhs) const;
    [[nodiscard]] gs_value operator>=(const gs_value& rhs) const;
    [[nodiscard]] gs_value operator!=(const gs_value& rhs) const;
    [[nodiscard]] bool is_truthy() const;

   private:
    void assert_matching_types(const gs_value& rhs) const;
};
[[nodiscard]] std::string to_string(const gs_value_variant& value);
[[nodiscard]] inline std::string to_string(const gs_value& value) {
    return to_string(value.value);
}
[[nodiscard]] inline std::string to_string(
    const std::optional<gs_value>& value) {
    if (value) {
        return to_string(value->value);
    }
    return "null";
}
}  // namespace GsInterpreter
#endif