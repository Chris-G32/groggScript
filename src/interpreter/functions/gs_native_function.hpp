//
// Created by Christopher Gruska on 11/8/25.
//

#ifndef GROGGSCRIPT_GS_NATIVE_FUNCTION_HPP
#define GROGGSCRIPT_GS_NATIVE_FUNCTION_HPP
#include <functional>
#include <optional>
#include <vector>

#include "abstract_gs_function.hpp"
namespace GsInterpreter {
using GsNativeFunctionImpl =
    std::function<std::optional<gs_value>(std::vector<gs_value>&)>;
class GsNativeFunction : public AbstractGsFunction {
   public:
    GsNativeFunction(
        const std::string& name,
        const std::vector<GSAlphabet::FunctionParameter>& parameters,
        const std::string& return_type, const GsNativeFunctionImpl& impl)
        : AbstractGsFunction(name, parameters, return_type), impl(impl) {}
    GsNativeFunction(GsNativeFunction&&) = default;
    GsNativeFunction(GsNativeFunction&) = default;
    ~GsNativeFunction() override = default;
    std::optional<gs_value> call(InterpreterVisitor* interpreter,
                                 std::vector<gs_value> args) override {
        return impl(args);
    }

   private:
    GsNativeFunctionImpl impl;
};

}  // namespace GsInterpreter

#endif  // GROGGSCRIPT_GS_NATIVE_FUNCTION_HPP
