#pragma once
#include <string>
#include <unordered_map>

#include "gs_value.hpp"
namespace GsInterpreter {
class GsEnvironment {
   public:
    GsEnvironment() = default;
    void initializeVariable(const std::string& identifier,
                            const GsValue& value);
    /**
        This is for when i bother with strict typing
      void initializeVariable(const std::string& identifier,
                            const GsValue& value);
     */

    void declareVariable(const std::string& identifier);
    void assignVariable(const std::string& identifier,
                        const std::optional<GsValue>& value);
    std::optional<GsValue> getVariable(const std::string& identifier);

    std::unordered_map<std::string, std::optional<GsValue>> _locals;
    std::unordered_map<std::string, std::optional<GsValue>> _globals;
};

}  // namespace GsInterpreter