//
// Created by Christopher Gruska on 11/23/25.
//
#include "gs_value.hpp"

#include <algorithm>
#include <functional>
#include <sstream>
#include <variant>

// template <class T1, class T2, class T3>
// constexpr bool both_same_as_v = std::is_same_v<T1, T3> && std::is_same_v<T2, T3>;
//
// template <class T1, class T2, class... SupportedType>
// constexpr bool same_and_supported =
//     std::is_same_v<T1, T2> && (both_same_as_v<T1, T2, SupportedType> || ...);
//
// template <class T, class... Ts>
// constexpr bool one_of_type = (std::is_same_v<T, Ts> || ...);
namespace GsInterpreter {

std::string to_string(const gs_value_type value) {
    switch (value) {
        case gs_value_type::BOOLEAN:
            return "boolean";
        case gs_value_type::FLOAT:
            return "decimal";
        case gs_value_type::INTEGER:
            return "int";
        case gs_value_type::STRING:
            return "string";
        case gs_value_type::OBJECT:
            return "object";
        case gs_value_type::ARRAY:
            return "array";
        case gs_value_type::FUNCTION:
            return "function";
        case gs_value_type::EMPTY:
            return "empty";
    }
    // Defensive fallback (should be impossible)
    return "UNKNOWN";
}
gs_value::gs_value(gs_value_variant val) : value(val) {
    std::visit(
        [this]<class ArgT>(ArgT&& arg) {
            using T = std::decay_t<ArgT>;
            if constexpr (std::is_same_v<T, gs_boolean>) {
                type = gs_value_type::BOOLEAN;
            } else if constexpr (std::is_same_v<T, gs_int>) {
                type = gs_value_type::INTEGER;
            } else if constexpr (std::is_same_v<T, gs_float>) {
                type = gs_value_type::FLOAT;
            } else if constexpr (std::is_same_v<T, gs_string>) {
                type = gs_value_type::STRING;
            } else if constexpr (std::is_same_v<T, gs_object>) {
                type = gs_value_type::OBJECT;
            } else if constexpr (std::is_same_v<T, gs_function>) {
                type = gs_value_type::FUNCTION;
            } else {
                type = gs_value_type::EMPTY;  // fallback for safety
            }
        },
        val);
}
std::string single_quoted(const std::string& value) { return "'" + value + "'"; }
std::string single_quoted(const gs_value_type& value) { return single_quoted(to_string(value)); }
void gs_value::assert_matching_types(const gs_value& rhs) const {
    if (type != rhs.type) {
        throw std::logic_error("Type mismatch. Expected: " + single_quoted(type) +
                               " Received: " + single_quoted(rhs.type));
    }
}
std::string to_string(const gs_value_variant& value) {
    return std::visit(
        []<typename T0>(T0&& arg) -> std::string {
            using T = std::decay_t<T0>;
            std::ostringstream oss;
            if constexpr (std::is_same_v<T, gs_boolean>) {
                oss << std::boolalpha;
            }
            if constexpr (one_of_type<T, gs_string, gs_int, gs_float, gs_boolean>) {
                oss << arg;
                return oss.str();
            }
            if constexpr (std::is_same_v<T, gs_array>) {
                bool outputComma = false;
                oss << "[ ";
                for (const auto& val : arg) {
                    if (outputComma) oss << ", ";
                    oss << to_string(val);
                    outputComma = true;
                }
                oss << " ]";
                return oss.str();
            }
            throw std::runtime_error("Not implemented");
        },
        value);
}
// template <class T1, class T2, class... Ts>
// concept same_and_supported =
//     std::is_same_v<T1, T2> &&
//     (std::is_same_v<T1, Ts> || ...);

template <class... SupportedTypes>
struct BinaryOpVisitor {
    template <class BinaryOp>
    static auto make(BinaryOp binaryOp, gs_value_type type, std::string_view op) {
        return [binaryOp, type, op]<typename T0, typename T1>(T0&& lhs, T1&& rhs) -> gs_value {
            using TL = std::decay_t<T0>;
            using TR = std::decay_t<T1>;

            if constexpr (std::is_same_v<TL, TR> && same_and_supported<TL, TR, SupportedTypes...>) {
                return gs_value(binaryOp(lhs, rhs));  // MUST return gs_value
            }
            std::ostringstream oss;
            oss << "Unsupported binary operation: " << op << " between values of type "
                << single_quoted(type) << '\n';
            throw std::logic_error(oss.str());
        };
    }
};

bool gs_value::is_truthy() const {
    if (type == gs_value_type::EMPTY) {
        return false;
    }

    return std::visit(
        []<typename T0>(const T0& val) -> bool {
            using T = std::decay_t<T0>;
            if constexpr (std::is_same_v<T, gs_boolean>) {
                return val;
            } else if constexpr (one_of_type<T, gs_string, gs_array>) {
                return val.empty();
            } else if constexpr (std::is_same_v<T, gs_float> || std::is_same_v<T, gs_int>) {
                return true;
            }  // TODO: Function and object truthiness.
            return false;
        },
        value);
}

}  // namespace GsInterpreter