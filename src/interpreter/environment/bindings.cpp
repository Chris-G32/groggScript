#include "bindings.hpp"
namespace GsInterpreter {
void throw_redeclare(const std::string& identifier) {
    throw std::runtime_error("Attempted to redeclare variable '" + identifier +
                             "'");
}
std::optional<gs_value>& Bindings::getSymbol(const std::string& identifier) {
    // Use at because this is in error if we try to get a non existent symbol
    return _symbols.at(identifier);
}
void Bindings::assignSymbol(const std::string& identifier,
                            std::optional<gs_value> value) {
    if (const auto& it = _symbols.find(identifier); it != _symbols.end()) {
        it->second = std::move(value);
        return;
    }
    throw std::runtime_error(
        "Attempted to assign a value to undefined variable '" + identifier +
        "'");
}
void Bindings::initializeSymbol(std::string identifier,
                                std::optional<gs_value> value) {
    if (const auto& it = _symbols.find(identifier); it == _symbols.end()) {
        _symbols.emplace(std::move(identifier), std::move(value));
        return;
    }
    throw_redeclare(identifier);
}
}  // namespace GsInterpreter