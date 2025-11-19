#pragma once
#include <variant>

#include "../concrete_syntax_tree/alphabet/primitive.hpp"

namespace GsInterpreter {
using GsValue = std::variant<GSAlphabet::Primitive, class AbstractGsFunction*>;
inline std::string to_string(const std::optional<GsValue>& value) {
    if (value == std::nullopt) {
        return "null";
    }
    if (std::holds_alternative<GSAlphabet::Primitive>(*value)) {
        return GSAlphabet::to_string(std::get<GSAlphabet::Primitive>(*value));
    }
    return "function";
}
}  // namespace GsInterpreter
