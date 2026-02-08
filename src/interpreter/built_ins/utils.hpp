//
// Created by Christopher Gruska on 11/24/25.
//

#ifndef GROGGSCRIPT_UTILS_HPP
#define GROGGSCRIPT_UTILS_HPP
#include <string>

#include "../../concrete_syntax_tree/alphabet/function_declaration.hpp"
#include "../gs_value.hpp"
namespace GsInterpreter {
class Params {
   public:
    static Params create() { return Params(); }
    static std::vector<GSAlphabet::FunctionParameter> create(gs_value_type type,
                                                             std::string name) {
        GSAlphabet::FunctionParameter p(name, to_string(type));
        return {p};
    }
    [[nodiscard]] Params& with(gs_value_type type, std::string name) {
        params.emplace_back(std::move(name), to_string(type));
        return *this;
    }
    std::vector<GSAlphabet::FunctionParameter> finalize() { return params; }

   private:
    std::vector<GSAlphabet::FunctionParameter> params;
    Params() = default;
    ~Params() = default;
};
}  // namespace GsInterpreter
#endif  // GROGGSCRIPT_UTILS_HPP
