#pragma once
#include <variant>

#include "../concrete_syntax_tree/alphabet/primitive.hpp"

namespace GsInterpreter {
using GsValue = std::variant<GSAlphabet::Primitive, class AbstractGsFunction*>;
}
