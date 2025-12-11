#ifndef GS_INTERPRETER_STATE_HPP
#define GS_INTERPRETER_STATE_HPP
#include <string>
#include <vector>

#include "bindings.hpp"

namespace GsInterpreter {

class InterpreterState {
   public:
    void push() { _stack.emplace_back(); }

    void push(Bindings state) { _stack.emplace_back(std::move(state)); }
    void pop() { _stack.pop_back(); }

    /// Gets the bindings to work with based on interpreter state, when there
    /// are no stack frames allocated, it will default to global. This should be
    /// the main one used, unless you know that you are executing in a local
    /// context.
    [[nodiscard]] constexpr Bindings& getDefaultScope() {
        return _stack.empty() ? globals : locals();
    }
    [[nodiscard]] constexpr Bindings& locals() { return _stack.back(); }
    Bindings globals;

   private:
    std::vector<Bindings> _stack;
};
struct InterpreterStateGuard {
    explicit InterpreterStateGuard(InterpreterState& state) : pState(state) {
        pState.push();
    }
    explicit InterpreterStateGuard(InterpreterState& state,
                                   const Bindings& bindings)
        : pState(state) {
        pState.push(bindings);
    }
    ~InterpreterStateGuard() { pState.pop(); }

   private:
    InterpreterState& pState;
};
}  // namespace GsInterpreter
#endif