#include "operator_specifications.hpp"

#include <algorithm>
#include <cassert>
#include <concepts>
#include <format>

#include "../interpreter/gs_value.hpp"
#include "../utils/type_utils.hpp"
namespace GsTruthTables {
using namespace GsInterpreter;
using Args = std::vector<gs_value>;

#define UNARY_OP_SUPPORTED(opname, op) \
    template <typename A>              \
    concept opname = requires(A a) {   \
        { a op };                      \
    }
#define BINARY_OP_SUPPORTED(opname, op)   \
    template <typename A, typename B>     \
    concept opname = requires(A a, B b) { \
        { a op b };                       \
    }
BINARY_OP_SUPPORTED(Subtractable, -);
BINARY_OP_SUPPORTED(Addable, +);
BINARY_OP_SUPPORTED(Multiplicable, *);
BINARY_OP_SUPPORTED(Divisible, /);

#include <concepts>
#include <stdexcept>

template <class T, class U>
    requires requires(const T& a, const U& b) {
        { a == b } -> std::convertible_to<bool>;
    }
gs_value op_eq_impl(const T& a, const U& b) {
    return gs_value(a == b);
}

// fallback (MUST NOT be a template)
inline gs_value op_eq_impl(...) { throw std::invalid_argument("== failed"); }

auto increment = [](const Args& args) -> gs_value {
    using namespace GsInterpreter;
    assert(args.size() == 1 && "Increment expects exactly one argument");
    const auto arg = args[0];
    assert(arg.type == gs_value_type::INTEGER &&
           "Increment only supports integers");  // Sanity check so that this fails with
    // readable message at runtime
    return gs_value(std::get<gs_int>(arg.value) + 1);
};
auto decrement = [](const Args& args) -> gs_value {
    using namespace GsInterpreter;
    assert(args.size() == 1 && "Increment expects exactly one argument");
    const auto arg = args[0];
    assert(arg.type == gs_value_type::INTEGER &&
           "Decrement only supports integers");  // Sanity check so that this fails with
    // readable message at runtime
    return gs_value(std::get<gs_int>(arg.value) - 1);
};
constexpr auto unaryOp = [](gs_value_type valType) {
    return std::make_tuple(valType, gs_value_type::EMPTY);
};

constexpr auto sameTypes = [](gs_value_type valType) { return std::make_tuple(valType, valType); };
std::vector integral = {sameTypes(gs_value_type::INTEGER), sameTypes(gs_value_type::FLOAT)};

template <typename Op>
gs_value binary_op(const Args& args, Op op, const char* opname) {
    assert(args.size() == 2);

    return std::visit(
        [&](const auto& a, const auto& b) -> gs_value {
            if constexpr (requires { op(a, b); }) {
                return gs_value(op(a, b));
            }
            throw std::invalid_argument(std::string(opname) + " failed");
        },
        args[0].value, args[1].value);
}
#define LOGICAL_OP_LAMBDA(op, opText, ...)                                                  \
    [](auto& args) {                                                                        \
        assert(args.size() == 2);                                                           \
        return std::visit(                                                                  \
            [&](const auto& a, const auto& b) -> gs_value {                                 \
                if constexpr (same_and_supported<std::decay_t<decltype(a)>,                 \
                                                 std::decay_t<decltype(b)>, __VA_ARGS__>) { \
                    return gs_value(a op b);                                                \
                }                                                                           \
                throw std::invalid_argument(std::string(opText) + " failed");               \
            },                                                                              \
            args[0].value, args[1].value);                                                  \
    }
template <typename Op>
gs_value unary_op(const Args& args, Op op, const char* opname) {
    assert(args.size() == 1);

    return std::visit(
        [&](const auto& a) -> gs_value {
            if constexpr (requires { op(a); }) {
                return gs_value(op(a));
            }
            throw std::invalid_argument(std::string(opname) + " failed");
        },
        args[0].value);
}
static std::string returnsLhs(const std::string& lhs, const std::string& rhs) { return lhs; }
auto returnsType(const gs_value_type& t) {
    return [t](const std::string& lhs, const std::string& rhs) { return to_string(t); };
}
static std::vector<OperatorSpecification> operatorDefinitions = {
    {.symbol = OperatorSymbol("++"),
     .arity = UNARY,
     .operation = increment,
     .supportedTypes = {unaryOp(gs_value_type::INTEGER)},
     .returnType = returnsLhs},
    {.symbol = OperatorSymbol("--"),
     .arity = UNARY,
     .operation = decrement,
     .supportedTypes = {unaryOp(gs_value_type::INTEGER)},
     .returnType = returnsLhs},
    {.symbol = OperatorSymbol("+"),
     .arity = BINARY,
     .operation = [](auto& a) { return binary_op(a, std::plus{}, "Add"); },
     .supportedTypes = {sameTypes(gs_value_type::INTEGER), sameTypes(gs_value_type::FLOAT),
                        sameTypes(gs_value_type::STRING)},
     .returnType = returnsLhs},
    {.symbol = OperatorSymbol("-"),
     .arity = BINARY,
     .operation = [](auto& a) { return binary_op(a, std::minus{}, "Subtract"); },
     .supportedTypes = integral,
     .returnType = returnsLhs},
    {.symbol = OperatorSymbol("*"),
     .arity = BINARY,
     .operation = [](auto& a) { return binary_op(a, std::multiplies{}, "Multiply"); },
     .supportedTypes = integral,
     .returnType = returnsLhs},
    {.symbol = OperatorSymbol("/"),
     .arity = BINARY,
     .operation = [](auto& a) { return binary_op(a, std::divides{}, "Divide"); },
     .supportedTypes = integral,
     .returnType = returnsLhs},
    {.symbol = OperatorSymbol("=="),
     .arity = BINARY,
     .operation =
         [](auto& args) {
             return std::visit(
                 [&](const auto& a, const auto& b) -> gs_value {
                     if constexpr (same_and_supported<std::decay_t<decltype(a)>,
                                                      std::decay_t<decltype(b)>, gs_boolean,
                                                      gs_float, gs_int, gs_string>) {
                         return gs_value(a == b);
                     }
                     throw std::invalid_argument("Unsupported types, operation fails");
                 },
                 args[0].value, args[1].value);
         },
     .supportedTypes = {sameTypes(gs_value_type::INTEGER), sameTypes(gs_value_type::FLOAT),
                        sameTypes(gs_value_type::STRING), sameTypes(gs_value_type::BOOLEAN)},
     .returnType = returnsType(gs_value_type::BOOLEAN)},
    {.symbol = OperatorSymbol("<"),
     .arity = BINARY,
     .operation = LOGICAL_OP_LAMBDA(<, "LessThan", gs_int, gs_float, gs_string),
     .supportedTypes = {sameTypes(gs_value_type::INTEGER), sameTypes(gs_value_type::FLOAT),
                        sameTypes(gs_value_type::STRING)},
     .returnType = returnsType(gs_value_type::BOOLEAN)},
    {.symbol = OperatorSymbol("<="),
     .arity = BINARY,
     .operation = LOGICAL_OP_LAMBDA(<=, "LessThanEquals", gs_int, gs_float, gs_string),
     .supportedTypes = {sameTypes(gs_value_type::INTEGER), sameTypes(gs_value_type::FLOAT),
                        sameTypes(gs_value_type::STRING)},
     .returnType = returnsType(gs_value_type::BOOLEAN)},
    {.symbol = OperatorSymbol(">"),
     .arity = BINARY,
     .operation = LOGICAL_OP_LAMBDA(<, "GreaterThan", gs_int, gs_float, gs_string),
     .supportedTypes = {sameTypes(gs_value_type::INTEGER), sameTypes(gs_value_type::FLOAT),
                        sameTypes(gs_value_type::STRING)},
     .returnType = returnsType(gs_value_type::BOOLEAN)}};
std::vector<OperatorSpecification> operatorSpecifications() { return operatorDefinitions; }
gs_value invoke(const OperatorSymbol symbol, const std::vector<gs_value>& args) {
    for (const auto& op : operatorDefinitions) {
        if (op.symbol == symbol) {
            return op.operation(args);
        }
    }
    throw std::runtime_error("Operator not found");
}
const OperatorSpecification& getBySymbol(OperatorSymbol symbol) {
    for (const auto& op : operatorDefinitions) {
        if (op.symbol == symbol) {
            return op;
        }
    }
    throw std::runtime_error("Operator not found");
}
}  // namespace GsTruthTables