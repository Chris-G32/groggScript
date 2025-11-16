#include "gs_user_function.hpp"

#include "../interpreter_visitor.hpp"

namespace GsInterpreter {
std::optional<GsValue> GsUserFunction::call(InterpreterVisitor* interpreter,
                                            std::vector<GsValue> args) {
    if (args.size() < parameters.size()) {
        throw std::runtime_error("Too few parameters provided for function: " +
                                 name);
    }
    for (size_t i = 0; i < parameters.size(); ++i) {
        try {
            interpreter->_environment.initializeVariable(parameters[i].name,
                                                         args[i]);
        } catch (...) {
            interpreter->_environment.assignVariable(parameters[i].name,
                                                     args[i]);
        }
    }
    interpreter->visit(body);
    return interpreter->popExpressionResult();
}
}  // namespace GsInterpreter