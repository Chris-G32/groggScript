//
// Created by Christopher Gruska on 11/11/25.
//

#ifndef GROGGSCRIPT_GS_USER_FUNCTION_HPP
#define GROGGSCRIPT_GS_USER_FUNCTION_HPP
#include <functional>
#include <optional>
#include <vector>

#include "../../concrete_syntax_tree/alphabet/function_declaration.hpp"
#include "../gs_value.hpp"
#include "abstract_gs_function.hpp"
namespace GsInterpreter {

class GsUserFunction : public AbstractGsFunction {
   public:
    GsUserFunction() = default;
    GsUserFunction(
        const std::string &name,
        const std::vector<GSAlphabet::FunctionParameter> &parameters,
        const std::string &return_type,
        const std::unique_ptr<GSAlphabet::AbstractAlphabetNode> &body)
        : AbstractGsFunction(name, parameters, return_type), body(body.get()) {}
    ~GsUserFunction() override = default;
    std::optional<gs_value> call(InterpreterVisitor *interpreter,
                                 std::vector<gs_value> args) override;
    GsUserFunction(GsUserFunction &&) = default;
    GsUserFunction(GsUserFunction &) = default;

   protected:
    GSAlphabet::AbstractAlphabetNode *body{};
};

}  // namespace GsInterpreter

#endif  // GROGGSCRIPT_GS_USER_FUNCTION_HPP
