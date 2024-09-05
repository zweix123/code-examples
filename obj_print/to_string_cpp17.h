#pragma once

#include <string>
#include <type_traits>

template<class T, class = void>
struct HasGetStringMethod : std::false_type {};
template<class T>
struct HasGetStringMethod<T, decltype(std::declval<T>().getString(), void())>
    : std::true_type {};

template<class T, class = void>
struct HasToStringMethod : std::false_type {};
template<class T>
struct HasToStringMethod<T, decltype(std::declval<T>().toString(), void())>
    : std::true_type {};

template<class T, class = void>
struct PtrCanGetStringMethod : std::false_type {};
template<class T>
struct PtrCanGetStringMethod<
    T,
    decltype(std::declval<T>()->getString(), void())> : std::true_type {};

template<class T, class = void>
struct PtrCanToStringMethod : std::false_type {};
template<class T>
struct PtrCanToStringMethod<T, decltype(std::declval<T>()->toString(), void())>
    : std::true_type {};

template<class T, class = void>
struct HasGetStringFunc : std::false_type {};
template<class T>
struct HasGetStringFunc<T, decltype(getString(std::declval<T>()), void())>
    : std::true_type {};

template<class T, class = void>
struct HasToStringFunc : std::false_type {};
template<class T>
struct HasToStringFunc<T, decltype(toString(std::declval<T>()), void())>
    : std::true_type {};

template<class T, class = void>
struct CanStdToStringFunc : std::false_type {};
template<class T>
struct CanStdToStringFunc<
    T,
    decltype(std::to_string(std::declval<T>()), void())> : std::true_type {};

struct NoMatchToString {};

template<class T>
decltype(auto) to_string_impl(T &&x) {
    if constexpr (HasGetStringMethod<T>::value) {
        return (std::forward<T>(x).getString());
    } else if constexpr (HasToStringMethod<T>::value) {
        return (std::forward<T>(x).toString());
    } else if constexpr (PtrCanGetStringMethod<T>::value) {
        return (std::forward<T>(x)->getString());
    } else if constexpr (PtrCanToStringMethod<T>::value) {
        return (std::forward<T>(x)->toString());
    } else if constexpr (HasGetStringFunc<T>::value) {
        return (getString(std::forward<T>(x)));
    } else if constexpr (HasToStringFunc<T>::value) {
        return (toString(std::forward<T>(x)));
    } else if constexpr (CanStdToStringFunc<T>::value) {
        return (std::to_string(std::forward<T>(x)));
    } else if constexpr (std::is_same<std::decay_t<T>, const char *>::value) {
        constexpr static const int CHARSTARTMAXLENGTH = 120;
        int endIdx = 0;
        for (; endIdx < CHARSTARTMAXLENGTH && x[endIdx]; ++endIdx)
            ;
        return std::string(x, endIdx);
    } else if constexpr (std::is_same<std::decay_t<T>, std::string>::value) {
        return (std::forward<T>(x));
    } else {
        return NoMatchToString{};
    }
}

template<class T>
auto to_string(T &&x) -> std::enable_if<
    !std::is_same<
        decltype(to_string_impl(std::declval<T>())),
        NoMatchToString>::value,
    decltype(to_string_impl(std::declval<T>()))>::type {
    return (to_string_impl(std::forward<T>(x)));
}