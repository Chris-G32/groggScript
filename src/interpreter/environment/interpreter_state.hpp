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
    // Commented out bc i dont want to redo if needed
    // [[nodiscard]] constexpr size_t depth() const { return _stack.size(); }

   private:
    std::vector<Bindings> _stack;
};

}  // namespace GsInterpreter
#endif