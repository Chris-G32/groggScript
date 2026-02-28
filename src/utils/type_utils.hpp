#ifndef GROGGSCRIPT_TYPE_UTILS_HPP
#define GROGGSCRIPT_TYPE_UTILS_HPP
#include <type_traits>
template <class T1, class T2, class T3>
constexpr bool both_same_as_v = std::is_same_v<T1, T3> && std::is_same_v<T2, T3>;

template <class T1, class T2, class... SupportedType>
constexpr bool same_and_supported =
    std::is_same_v<T1, T2> && (both_same_as_v<T1, T2, SupportedType> || ...);

template <class T, class... Ts>
constexpr bool one_of_type = (std::is_same_v<T, Ts> || ...);

template <class T, class... Ts>
constexpr bool is_alternative_of = (std::is_same_v<T, Ts> || ...);

template <typename T, typename Variant>
concept InVariant = []<typename... Ts>(std::variant<Ts...>*) {
    return (std::is_same_v<T, Ts> || ...);
}(static_cast<Variant*>(nullptr));
#endif  // GROGGSCRIPT_TYPE_UTILS_HPP
