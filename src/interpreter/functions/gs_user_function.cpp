#include "gs_user_function.hpp"

#include "../interpreter_visitor.hpp"

namespace GsInterpreter {
std::optional<gs_value> GsUserFunction::call(InterpreterVisitor* interpreter,
                                             std::vector<gs_value> args) {
    if (args.size() < parameters.size()) {
        throw std::runtime_error("Too few parameters provided for function: " +
                                 name);
    }
    auto bindings = new Bindings();
    for (size_t i = 0; i < parameters.size(); ++i) {
        bindings->initializeSymbol(parameters[i].name, args[i]);
    }
    interpreter->mEnvironment_.push(bindings);
    interpreter->visit(body);
    interpreter->mEnvironment_.pop();
    return interpreter->_exprResult;
}
}  // namespace GsInterpreter