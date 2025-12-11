#include "bindings.hpp"
namespace GsInterpreter {
void throw_redeclare(const std::string& identifier) {
    throw std::runtime_error("Attempted to redeclare variable '" + identifier +
                             "'");
}
std::optional<gs_value>& Bindings::getSymbol(const std::string& identifier) {
    if (mSymbols_.contains(identifier)) {
        return mSymbols_.at(identifier);
    }
    if (mParent_) {
        return mParent_->getSymbol(identifier);
    }
    throw std::runtime_error("Symbol '" + identifier + "not declared in scope");
}
void Bindings::assignSymbol(const std::string& identifier,
                            std::optional<gs_value> value) {
    if (const auto& it = mSymbols_.find(identifier); it != mSymbols_.end()) {
        it->second = std::move(value);
        return;
    }
    throw std::runtime_error(
        "Attempted to assign a value to undefined variable '" + identifier +
        "'");
}
void Bindings::initializeSymbol(std::string identifier,
                                std::optional<gs_value> value) {
    if (const auto& it = mSymbols_.find(identifier); it == mSymbols_.end()) {
        mSymbols_.emplace(std::move(identifier), std::move(value));
        return;
    }
    throw_redeclare(identifier);
}
}  // namespace GsInterpreter