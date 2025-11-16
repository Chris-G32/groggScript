#ifndef GROGGSCRIPT_FUNCTION_DECLARATION_HPP
#define GROGGSCRIPT_FUNCTION_DECLARATION_HPP
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "abstract_alphabet_node.hpp"
#include "symbol.hpp"

using std::string;

namespace GSAlphabet {
struct FunctionParameter {
    string name;
    string type;
};

class FunctionDeclaration : public AbstractAlphabetNode {
   public:
    explicit FunctionDeclaration(const std::string& name,
                                 std::unique_ptr<AbstractAlphabetNode> body)
        : FunctionDeclaration(name, {}, std::move(body)) {}
    FunctionDeclaration(std::string name, std::vector<FunctionParameter> args,
                        std::unique_ptr<AbstractAlphabetNode> body,
                        std::optional<string> returnType = std::nullopt)
        : body(std::move(body)),
          arguments(std::move(args)),
          returnType(std::move(returnType)),
          name(std::move(name)) {}
    void accept(AbstractAlphabetNodeVisitor* visitor) override {
        visitor->visitFunctionDeclaration(this);
    };
    std::string name;
    std::unique_ptr<AbstractAlphabetNode> body;
    std::vector<FunctionParameter> arguments;
    std::optional<std::string> returnType;
};
}  // namespace GSAlphabet
#endif  // GROGGSCRIPT_FUNCTION_DECLARATION_HPP
