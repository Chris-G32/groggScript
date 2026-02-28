//
// Created by Christopher Gruska on 11/24/25.
//

#include "built_in_functions.hpp"

#include <iostream>
#include <ostream>

#include "../functions/gs_native_function.hpp"
#include "utils.hpp"
namespace GsInterpreter {
GsNativeFunction GsBuiltIns::print("print", Params::create(gs_value_type::STRING, "text"), "void",
                                   [](const std::vector<gs_value>& args) {
                                       std::cout << to_string(args.at(0)) << std::endl;
                                       return std::nullopt;
                                   });
GsNativeFunction GsBuiltIns::toString(
    "toString", Params::create(gs_value_type::OBJECT, "val"), "string",
    [](const std::vector<gs_value>& args) -> std::optional<gs_value> {
        return gs_value(to_string(args[0]));
    });
GsNativeFunction GsBuiltIns::at("at", Params::create(gs_value_type::STRING, "val"), "string",
                                [](const std::vector<gs_value>& args) -> std::optional<gs_value> {
                                    return gs_value(
                                        args[0].get<gs_string>().at(args[1].get<gs_int>()));
                                });
GsNativeFunction GsBuiltIns::atArray("atArr", Params::create(gs_value_type::ARRAY, "val"), "string",
                                     [](const std::vector<gs_value>& args)
                                         -> std::optional<gs_value> {
                                         return args[0].get<gs_array>().at(args[1].get<gs_int>());
                                     });

GsNativeFunction GsBuiltIns::length(
    "length", Params::create(gs_value_type::STRING, "val"), "int",
    [](const std::vector<gs_value>& args) -> std::optional<gs_value> {
        return gs_value(static_cast<gs_int>(args[0].get<gs_string>().size()));
    });
// string []
// dict
// array
// Arrays
// make
// at
// size()
}  // namespace GsInterpreter