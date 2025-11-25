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
    [[nodiscard]] std::optional<gs_value>& getSymbol(
        const std::string& identifier);
    /// Introduces this symbol, stops it getting redefined in the same scope
    void declareSymbol(const std::string& identifier) {
        initializeSymbol(identifier, std::nullopt);
    }
    /// Assign a value to an already initialized symbol
    void assignSymbol(const std::string& identifier,
                      std::optional<gs_value> value);
    /// Declare and assign a value
    void initializeSymbol(std::string identifier,
                          std::optional<gs_value> value);

   private:
    std::unordered_map<std::string, std::optional<gs_value>> _symbols;
};

}  // namespace GsInterpreter
#endif  // GROGGSCRIPT_BINDINGS_HPP
