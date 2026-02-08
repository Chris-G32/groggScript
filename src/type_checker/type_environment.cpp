//
// Created by Christopher Gruska on 12/26/25.
//

#include "type_environment.hpp"

namespace GsInterpreter {
void TypeEnvironment::pushScope() {
    // explicit because it is easy to accidentally use the copy ctor
    TypeBindings* parent = &activeScope();
    locals_.push_back(new TypeBindings(parent));
}
void TypeEnvironment::popScope() {
    if (locals_.empty()) {
        return;
    }
    delete locals_.back();
    locals_.pop_back();
}
TypeBindings& TypeEnvironment::activeScope() {
    return locals_.empty() ? globals_ : *locals_.back();
}
}  // namespace GsInterpreter