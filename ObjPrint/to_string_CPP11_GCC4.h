#pragma once

#include <string>
#include <type_traits>

template<typename T>
struct has_toString_method {
    template<typename U>
    static auto test(int)
        -> decltype(std::declval<U>().toString(), std::true_type{});

    template<typename>
    static auto test(...) -> std::false_type;

    static constexpr bool value = decltype(test<T>(0))::value;
};
template<typename T>
struct has_getString_method {
    template<typename U>
    static auto test(int)
        -> decltype(std::declval<U>().getString(), std::true_type{});

    template<typename>
    static auto test(...) -> std::false_type;

    static constexpr bool value = decltype(test<T>(0))::value;
};
template<typename T>
struct can_toString_method {
    template<typename U>
    static auto test(int)
        -> decltype(std::declval<U>()->toString(), std::true_type{});

    template<typename>
    static auto test(...) -> std::false_type;

    static constexpr bool value = decltype(test<T>(0))::value;
};
template<typename T>
struct can_getString_method {
    template<typename U>
    static auto test(int)
        -> decltype(std::declval<U>()->getString(), std::true_type{});

    template<typename>
    static auto test(...) -> std::false_type;

    static constexpr bool value = decltype(test<T>(0))::value;
};
template<typename T>
struct has_toString_function {
    template<typename U>
    static auto test(int)
        -> decltype(toString(std::declval<U>()), std::true_type{});

    template<typename>
    static auto test(...) -> std::false_type;

    static constexpr bool value = decltype(test<T>(0))::value;
};
template<typename T>
struct has_getString_function {
    template<typename U>
    static auto test(int)
        -> decltype(getString(std::declval<U>()), std::true_type{});

    template<typename>
    static auto test(...) -> std::false_type;

    static constexpr bool value = decltype(test<T>(0))::value;
};
template<typename T>
typename std::enable_if<has_toString_method<T>::value, std::string>::type
to_string(const T &x) {
    return x.toString();
}

template<typename T>
typename std::enable_if<has_getString_method<T>::value, std::string>::type
to_string(const T &x) {
    return x.getString();
}

template<typename T>
typename std::enable_if<can_toString_method<T>::value, std::string>::type
to_string(const T &x) {
    return x->toString();
}

template<typename T>
typename std::enable_if<can_getString_method<T>::value, std::string>::type
to_string(const T &x) {
    return x->getString();
}

template<typename T>
typename std::enable_if<has_toString_function<T>::value, std::string>::type
to_string(const T &x) {
    return toString(x);
}

template<typename T>
typename std::enable_if<has_getString_function<T>::value, std::string>::type
to_string(const T &x) {
    return getString(x);
}

template<typename T>
typename std::enable_if<
    !has_toString_method<T>::value && !has_getString_method<T>::value
        && !can_toString_method<T>::value && !can_getString_method<T>::value
        && !has_toString_function<T>::value
        && !has_getString_function<T>::value,
    std::string>::type
to_string(const T &x) {
    return std::to_string(x);
}

inline std::string to_string(const char *const &x) {
    constexpr static const int CHARSTARTMAXLENGTH = 120;
    int endIdx = 0;
    for (; endIdx < CHARSTARTMAXLENGTH && x[endIdx]; ++endIdx)
        ;
    return std::string(x, endIdx);
}
inline std::string to_string(const std::string &s) {
    return s;
}