#ifndef GS_INTERPRETER_STATE_HPP
#define GS_INTERPRETER_STATE_HPP
#include <vector>

#include "bindings.hpp"

namespace GsInterpreter {

class InterpreterState {
   public:
    void push() { _stack.emplace_back(); }

    void push(Bindings* state) { _stack.push_back(state); }
    void pop() {
        delete _stack.back();
        _stack.pop_back();
    }

    /// Gets the bindings to work with based on interpreter state, when there
    /// are no stack frames allocated, it will default to global. This should be
    /// the main one used, unless you know that you are executing in a local
    /// context.
    [[nodiscard]] constexpr Bindings& getDefaultScope() {
        return _stack.empty() ? globals : locals();
    }
    [[nodiscard]] constexpr Bindings& locals() { return *_stack.back(); }
    Bindings globals;

   private:
    /// This causes invalidation of pointers on copy, I need a different way to
    /// associate bindings, maybe this needs to be a pointer array
    std::vector<Bindings*> _stack;
};
struct InterpreterStateGuard {
    explicit InterpreterStateGuard(InterpreterState& state) : pState(state) {
        pState.push(new Bindings());
    }
    explicit InterpreterStateGuard(InterpreterState& state, Bindings* bindings)
        : pState(state) {
        pState.push(bindings);
    }
    ~InterpreterStateGuard() { pState.pop(); }

   private:
    InterpreterState& pState;
};
}  // namespace GsInterpreter
#endif