#ifndef GS_VALUE_HPP_
#define GS_VALUE_HPP_
#include <sstream>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include "../utils/type_utils.hpp"
namespace GsInterpreter {
enum class gs_value_type { BOOLEAN, FLOAT, INTEGER, STRING, OBJECT, ARRAY, FUNCTION, EMPTY };
std::string to_string(gs_value_type value);
struct gs_value;
using gs_array = std::vector<gs_value>;
using gs_object = std::unordered_map<std::string, gs_value>;
using gs_int = long long;
using gs_float = double;
using gs_boolean = bool;
using gs_string = std::string;
using gs_function = class AbstractGsFunction*;

using gs_value_variant =
    std::variant<gs_int, gs_float, gs_boolean, gs_string, gs_function, gs_object, gs_array>;
struct gs_value {  // TODO: Switch to using unique ptr, or maybe the object
                   // itself if possible. There is a mem leak as is.
    explicit gs_value(gs_value_variant val);
    gs_value() { type = gs_value_type::EMPTY; }
    gs_value_type type;
    gs_value_variant value;
    [[nodiscard]] bool is_truthy() const;
    template <typename T>
    const T& get() const {
        return std::get<T>(value);
    }

   private:
    void assert_matching_types(const gs_value& rhs) const;
};
[[nodiscard]] std::string to_string(const gs_value_variant& value);
[[nodiscard]] inline std::string to_string(const gs_value& value) { return to_string(value.value); }
[[nodiscard]] inline std::string to_string(const std::optional<gs_value>& value) {
    if (value) {
        return to_string(value);
    }
    return "null";
}
}  // namespace GsInterpreter
#endif