#ifndef GROGGSCRIPT_GS_FUNCTION_HPP
#define GROGGSCRIPT_GS_FUNCTION_HPP

#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "../../concrete_syntax_tree/alphabet/function_declaration.hpp"
#include "../gs_value.hpp"
namespace GsInterpreter {
class AbstractGsFunction {
   public:
    AbstractGsFunction() = default;
    AbstractGsFunction(std::string name,
                       std::vector<GSAlphabet::FunctionParameter> parameters,
                       std::string return_type)
        : name(std::move(name)),
          parameters(std::move(parameters)),
          returnType(std::move(return_type)) {}
    virtual ~AbstractGsFunction() = default;
    [[nodiscard]] size_t arity() const { return parameters.size(); }
    virtual std::optional<gs_value> call(class InterpreterVisitor* interpreter,
                                         std::vector<gs_value> args) = 0;
    std::string name;
    std::vector<GSAlphabet::FunctionParameter> parameters;
    std::string returnType;
};
}  // namespace GsInterpreter
#endif  // GROGGSCRIPT_GS_FUNCTION_HPP
