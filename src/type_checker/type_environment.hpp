//
// Created by Christopher Gruska on 12/26/25.
//

#ifndef GROGGSCRIPT_TYPE_ENVIRONMENT_HPP
#define GROGGSCRIPT_TYPE_ENVIRONMENT_HPP
#include <vector>

#include "type_bindings.hpp"
namespace GsInterpreter {

class TypeEnvironment {
   public:
    void pushScope();
    void popScope();
    TypeBindings& activeScope();

   private:
    TypeBindings globals_;
    std::vector<TypeBindings*> locals_;
};
class TypeEnvironmentScopeGuard {
   public:
    explicit TypeEnvironmentScopeGuard(TypeEnvironment& env) : env_(env) { env.pushScope(); }
    ~TypeEnvironmentScopeGuard() { env_.popScope(); }

   private:
    TypeEnvironment& env_;
};
}  // namespace GsInterpreter

#endif  // GROGGSCRIPT_TYPE_ENVIRONMENT_HPP
