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
struct has_std_to_string_function {
    template<typename U>
    static auto test(int)
        -> decltype(std::to_string(std::declval<U>()), std::true_type{});

    template<typename>
    static auto test(...) -> std::false_type;

    static constexpr bool value = decltype(test<T>(0))::value;
};

template<typename T>
typename std::enable_if<has_toString_method<T>::value, std::string>::type
to_string_impl(const T &x) {
    return x.toString();
}

template<typename T>
typename std::enable_if<has_getString_method<T>::value, std::string>::type
to_string_impl(const T &x) {
    return x.getString();
}

template<typename T>
typename std::enable_if<can_toString_method<T>::value, std::string>::type
to_string_impl(const T &x) {
    return x->toString();
}

template<typename T>
typename std::enable_if<can_getString_method<T>::value, std::string>::type
to_string_impl(const T &x) {
    return x->getString();
}

template<typename T>
typename std::enable_if<has_toString_function<T>::value, std::string>::type
to_string_impl(const T &x) {
    return toString(x);
}

template<typename T>
typename std::enable_if<has_getString_function<T>::value, std::string>::type
to_string_impl(const T &x) {
    return getString(x);
}

template<typename T>
typename std::enable_if<has_std_to_string_function<T>::value, std::string>::type
to_string_impl(const T &x) {
    return std::to_string(x);
}

template<typename T>
typename std::enable_if<
    std::is_same<typename std::decay<T>::type, const char *>::value,
    std::string>::type
to_string_impl(const T &x) {
    constexpr static const int CHARSTARTMAXLENGTH = 120;
    int endIdx = 0;
    for (; endIdx < CHARSTARTMAXLENGTH && x[endIdx]; ++endIdx)
        ;
    return std::string(x, endIdx);
}

template<typename T>
typename std::enable_if<
    std::is_same<typename std::decay<T>::type, std::string>::value,
    std::string>::type
to_string_impl(const T &x) {
    return x;
}

struct NoMatchToString {};

template<typename T>
typename std::enable_if<
    !has_toString_method<T>::value && !has_getString_method<T>::value
        && !can_toString_method<T>::value && !can_getString_method<T>::value
        && !has_toString_function<T>::value && !has_getString_function<T>::value
        && !has_std_to_string_function<T>::value
        && !std::is_same<typename std::decay<T>::type, const char *>::value
        && !std::is_same<typename std::decay<T>::type, std::string>::value,
    NoMatchToString>::type
to_string_impl(const T &x) {
    return NoMatchToString{};
}

template<class T>
auto to_string(T &&x) -> std::enable_if<
    !std::is_same<
        decltype(to_string_impl(std::declval<T>())),
        NoMatchToString>::value,
    decltype(to_string_impl(std::declval<T>()))>::type {
    return (to_string_impl(std::forward<T>(x)));
}
