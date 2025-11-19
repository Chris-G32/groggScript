#ifndef GROGGSCRIPT_BINDINGS_HPP
#define GROGGSCRIPT_BINDINGS_HPP
#include <string>
#include <unordered_map>

#include "../gs_value.hpp"

namespace GsInterpreter {
class Bindings {
   public:
    Bindings() {
        // assume maybe 10 symbols is the average case for locals, avoid rehash
        _symbols.reserve(10);
    }
    [[nodiscard]] std::optional<GsValue>& getSymbol(
        const std::string& identifier);
    /// Introduces this symbol, stops it getting redefined in the same scope
    void declareSymbol(const std::string& identifier) {
        initializeSymbol(identifier, std::nullopt);
    }
    /// Assign a value to an already initialized symbol
    void assignSymbol(const std::string& identifier,
                      std::optional<GsValue> value);
    /// Declare and assign a value
    void initializeSymbol(std::string identifier, std::optional<GsValue> value);

   private:
    std::unordered_map<std::string, std::optional<GsValue>> _symbols;
};

}  // namespace GsInterpreter
#endif  // GROGGSCRIPT_BINDINGS_HPP
