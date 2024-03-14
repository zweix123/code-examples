#pragma once

#include <string>
#include <type_traits>

template<class... _Types>
using void_t = void;

template<typename T, std::size_t N = 0, typename = void>
struct to_string_dispatcher : to_string_dispatcher<T, N + 1> {};

// object.toString
template<typename T>
struct to_string_dispatcher<
    T,
    0,
    void_t<decltype(std::declval<T>().toString())>> {
    static_assert(
        std::is_same<decltype(std::declval<T>().toString()), std::string>::
            value,
        "");
    template<typename U>
    static std::string invoke(U &&arg) {
        return std::forward<U>(arg).toString();
    }
};

// object.getString
template<typename T>
struct to_string_dispatcher<
    T,
    1,
    void_t<decltype(std::declval<T>().getString())>> {
    static_assert(
        std::is_same<decltype(std::declval<T>().getString()), std::string>::
            value,
        "");
    template<typename U>
    static std::string invoke(U &&arg) {
        return std::forward<U>(arg).getString();
    }
};

// pointer -> toString
template<typename T>
struct to_string_dispatcher<
    T,
    2,
    void_t<decltype(std::declval<T>()->toString())>> {
    static_assert(
        std::is_same<decltype(std::declval<T>()->toString()), std::string>::
            value,
        "");
    template<typename U>
    static std::string invoke(U &&arg) {
        return std::forward<U>(arg)->toString();
    }
};

// pointer -> getString
template<typename T>
struct to_string_dispatcher<
    T,
    3,
    void_t<decltype(std::declval<T>()->getString())>> {
    static_assert(
        std::is_same<decltype(std::declval<T>()->getString()), std::string>::
            value,
        "");
    template<typename U>
    static std::string invoke(U &&arg) {
        return std::forward<U>(arg)->getString();
    }
};

// toString(object)
template<typename T>
struct to_string_dispatcher<
    T,
    4,
    void_t<decltype(toString(std::declval<T>()))>> {
    static_assert(
        std::is_same<decltype(toString(std::declval<T>())), std::string>::value,
        "");
    template<typename U>
    static std::string invoke(U &&arg) {
        return toString(std::forward<U>(arg));
    }
};

// getString(object)
template<typename T>
struct to_string_dispatcher<
    T,
    5,
    void_t<decltype(getString(std::declval<T>()))>> {
    static_assert(
        std::is_same<decltype(getString(std::declval<T>())), std::string>::
            value,
        "");
    template<typename U>
    static std::string invoke(U &&arg) {
        return getString(std::forward<U>(arg));
    }
};

template<typename T>
struct to_string_dispatcher<
    T,
    6,
    typename std::enable_if<
        std::is_same<typename std::decay<T>::type, std::string>::value>::type> {
    template<typename U>
    static std::string invoke(U &&arg) {
        return std::forward<U>(arg);
    }
};

template<typename T>
struct to_string_dispatcher<
    T,
    7,
    void_t<decltype(std::to_string(std::declval<T>()))>> {
    static_assert(
        std::is_same<decltype(std::to_string(std::declval<T>())), std::string>::
            value,
        "");
    template<typename U>
    static std::string invoke(U &&arg) {
        return std::to_string(std::forward<U>(arg));
    }
};

template<typename T>
struct to_string_dispatcher<
    T,
    8,
    typename std::enable_if<
        std::is_same<typename std::decay<T>::type, const char *>::value>::
        type> {
    template<typename U>
    static std::string invoke(U &&arg) {
        constexpr static const int CHARSTARTMAXLENGTH = 120;
        int endIdx = 0;
        for (; endIdx < CHARSTARTMAXLENGTH && arg[endIdx]; ++endIdx)
            ;
        return std::string(arg, endIdx);
    }
};

template<typename T>
struct to_string_dispatcher<T, 9, void> {
    template<typename U>
    static void invoke(U &&arg) {
        //
        static_assert(false, "unsupported type");
    }
};

template<typename T>
std::string to_string(T &&x) {
    return to_string_dispatcher<T>::invoke(std::forward<T>(x));
}
