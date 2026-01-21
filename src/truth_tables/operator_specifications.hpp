#ifndef GROGGSCRIPT_BINARY_OPERATORS_HPP
#define GROGGSCRIPT_BINARY_OPERATORS_HPP

#include <array>
#include <functional>
#include <string>
#include <variant>

#include "../concrete_syntax_tree/alphabet/binary_expression.hpp"
#include "../concrete_syntax_tree/alphabet/unary_expression.hpp"

namespace GsInterpreter {
enum class gs_value_type;
struct gs_value;
gs_value dummy(const std::vector<gs_value>&);
}  // namespace GsInterpreter

namespace GsTruthTables {
struct OperatorSymbol {
    explicit OperatorSymbol(const std::string& symbolStr) {
        if (symbolStr.size() > symbol.size()) {
            throw std::runtime_error("Operator symbol is too long");
        }
        int index = 0;
        for (const auto& c : symbolStr) {
            symbol[index++] = c;
        }
    }
    bool operator==(const OperatorSymbol& rhs) const {
        for (int i = 0; i < symbol.size(); i++) {
            if (symbol[i] != rhs.symbol[i]) {
                return false;
            }
        }
        return true;
    }
    std::array<char, 2> symbol = {'\0', '\0'};
};
inline unsigned char UNARY = 1;
inline unsigned char BINARY = 2;
inline OperatorSymbol fromAst(const GSAlphabet::BinaryOperator& op) {
    using OpSym = GsTruthTables::OperatorSymbol;
    using enum GSAlphabet::BinaryOperator;
    switch (op) {
        case LOGICAL_OR:
            return OpSym("||");
        case LOGICAL_AND:
            return OpSym("&&");
        case LOGICAL_EQUALS:
            return OpSym("==");
        case ADDITION:
            return OpSym("+");
        case SUBTRACTION:
            return OpSym("-");
        case MULTIPLY:
            return OpSym("*");
        case DIVISION:
            return OpSym("/");
        case LESS_THAN:
            return OpSym("<");
        case GREATER_THAN:
            return OpSym(">");
        case LESS_THAN_EQUALS:
            return OpSym("<=");
        case GREATER_THAN_EQUALS:
            return OpSym(">=");
        default:
            throw std::runtime_error("Unsupported binary operator");
    }
}
inline OperatorSymbol fromAst(const GSAlphabet::UnaryOperator& op) {
    using OpSym = GsTruthTables::OperatorSymbol;
    using enum GSAlphabet::UnaryOperator;
    switch (op) {
        case LOGICAL_NOT:
            return OpSym("!");
        case INCREMENT:
            return OpSym("++");
        case DECREMENT:
            return OpSym("--");
        default:
            throw std::runtime_error("Unsupported unary operator");
    }
}
struct OperatorSpecification {
    OperatorSymbol symbol;
    unsigned char arity;  // Usually 1 or 2
    std::function<GsInterpreter::gs_value(const std::vector<GsInterpreter::gs_value>&)> operation;
    // As of now there is no ternary operator supported. So two suffices. No meaning is held in
    // second option when arity is 1
    std::vector<std::tuple<GsInterpreter::gs_value_type, GsInterpreter::gs_value_type>>
        supportedTypes;
};
std::vector<OperatorSpecification> operatorSpecifications();
GsInterpreter::gs_value invoke(const OperatorSymbol symbol,
                               const std::vector<GsInterpreter::gs_value>& args);
const OperatorSpecification& getBySymbol(OperatorSymbol symbol);
}  // namespace GsTruthTables
#endif  // GROGGSCRIPT_BINARY_OPERATORS_HPP
