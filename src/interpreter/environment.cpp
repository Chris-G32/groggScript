#include "environment.hpp"

#include <optional>
#include <string>

#include "gs_value.hpp"
namespace GsInterpreter {
void throw_redeclare(const std::string& identifier) {
    throw std::runtime_error("Attempted to redeclare variable '" + identifier +
                             "'");
}
void GsEnvironment::initializeVariable(const std::string& identifier,
                                       const GsValue& value) {
    if (const auto it = _locals.find(identifier); it == _locals.end()) {
        _locals.emplace(identifier, value);
        return;
    }
    throw_redeclare(identifier);
}
void GsEnvironment::declareVariable(const std::string& identifier) {
    if (const auto it = _locals.find(identifier); it == _locals.end()) {
        _locals.emplace(identifier, std::nullopt);
        return;
    }
    throw_redeclare(identifier);
}
void GsEnvironment::assignVariable(const std::string& identifier,
                                   const std::optional<GsValue>& value) {
    if (auto it = _locals.find(identifier); it != _locals.end()) {
        it->second = value;
        return;
    }
    throw std::runtime_error(
        "Attempted to assign a value to undefined variable '" + identifier +
        "'");
}
std::optional<GsValue> GsEnvironment::getVariable(
    const std::string& identifier) {
    if (auto variable = _locals.find(identifier); variable != _locals.end()) {
        return variable->second;
    }
    throw std::runtime_error("Symbol '" + identifier + "' is not defined");
}
}  // namespace GsInterpreter