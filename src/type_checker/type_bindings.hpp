//
// Created by Christopher Gruska on 12/26/25.
//

#ifndef GROGGSCRIPT_TYPE_BINDINGS_HPP
#define GROGGSCRIPT_TYPE_BINDINGS_HPP
#include <map>
#include <string>
#include <unordered_map>

namespace GsInterpreter {

class TypeBindings {
   public:
    /// Constant representing a symbol that is not contained in the bindings type
    inline static const std::string UNBOUND_TYPE = "";
    explicit TypeBindings(TypeBindings* parent = nullptr) : parent_(parent) {}

    /// Bind a symbol to a typename
    /// @param symbol Symbol to bind
    /// @param typeName Type of the symbol
    /// @returns True when the symbol does not already have a type bound to it,
    /// false when binding fails
    [[nodiscard]] bool bind(const std::string& symbol, const std::string& typeName);

    /// Gets the type of a symbol
    /// @param symbol Symbol to retrieve type of
    /// @returns UNBOUND_TYPE when the symbol is not contained in the bindings. The type otherwise.
    [[nodiscard]] std::string typeOf(const std::string& symbol) const;

    /// Check if the symbol matches a specified type
    /// @param symbol Symbol to check
    /// @param typeName Type to check for a match
    /// @return True when symbol is contained by these bindings or one of its parent nodes
    /// and matches specified type name, false otherwise
    [[nodiscard]] bool matches(const std::string& symbol, const std::string& typeName) const;

   private:
    TypeBindings* parent_;
    std::unordered_map<std::string, std::string> bindings_;
};

}  // namespace GsInterpreter

#endif  // GROGGSCRIPT_TYPE_BINDINGS_HPP
