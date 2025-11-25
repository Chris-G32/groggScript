//
// Created by Christopher Gruska on 11/24/25.
//

#include "built_in_functions.hpp"

#include <iostream>
#include <ostream>

#include "../functions/gs_native_function.hpp"
#include "utils.hpp"
namespace GsInterpreter {
GsNativeFunction GsBuiltIns::print(
    "print", Params::create(gs_value_type::STRING, "text"), "void",
    [](const std::vector<gs_value>& args) {
        std::cout << to_string(args.at(0)) << std::endl;
        return std::nullopt;
    });
GsNativeFunction GsBuiltIns::toString(
    "toString", Params::create(gs_value_type::OBJECT, "val"), "string",
    [](std::vector<gs_value>& args) -> std::optional<gs_value> {
        return gs_value(to_string(args[0]));
    });
}  // namespace GsInterpreter