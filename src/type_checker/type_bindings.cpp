//
// Created by Christopher Gruska on 12/26/25.
//

#include "type_bindings.hpp"

namespace GsInterpreter {
bool TypeBindings::bind(const std::string& symbol, const std::string& typeName) {
    if (bindings_.contains(symbol)) {
        return false;
    }
    bindings_[symbol] = typeName;
    return true;
}
std::string TypeBindings::typeOf(const std::string& symbol) const {
    if (const auto it = bindings_.find(symbol); it != bindings_.end()) {
        return it->second;
    }
    if (parent_ != nullptr) {
        return parent_->typeOf(symbol);
    }
    return UNBOUND_TYPE;
}
bool TypeBindings::matches(const std::string& symbol, const std::string& typeName) const {
    if (const auto it = bindings_.find(symbol); it != bindings_.end()) {
        return it->second == typeName;
    }
    if (parent_ != nullptr) {
        return parent_->matches(symbol, typeName);
    }
    return false;
}
}  // namespace GsInterpreter