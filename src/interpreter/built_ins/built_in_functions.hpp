//
// Created by Christopher Gruska on 11/24/25.
//

#ifndef GROGGSCRIPT_BUILT_IN_FUNCTIONS_HPP
#define GROGGSCRIPT_BUILT_IN_FUNCTIONS_HPP
#include <iostream>
#include <ostream>

#include "../functions/gs_native_function.hpp"
#include "utils.hpp"
namespace GsInterpreter {

class GsBuiltIns {
   public:
    static GsNativeFunction print;

    static GsNativeFunction toString;
    static GsNativeFunction at;
    static GsNativeFunction atArray;
    static GsNativeFunction length;
};
}  // namespace GsInterpreter
#endif  // GROGGSCRIPT_BUILT_IN_FUNCTIONS_HPP
