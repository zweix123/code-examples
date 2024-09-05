#pragma once

#include <array>
#include <cassert>
#include <iostream>
#include <map>
#include <optional>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>

namespace ObjPrint {

// #define MultiMethod
// #define DuplicateNameGetString
// #define DuplicateNameToString

static int depth = -1;

// helper
inline std::string genTabStr(int tabNum) {
    return std::string(tabNum << 1, ' ');
}

inline std::string shift(const std::string &s, const std::string &pre) {
    std::stringstream ss(s);
    std::vector<std::string> lines;
    {
        std::string line;
        while (std::getline(ss, line)) {
            lines.push_back(std::move(line));
        }
    }
    std::string result;
    {
        bool first = true;
        for (const auto &line : lines) {
            if (first) first = false;
            else
                result += "\n" + pre;
            result += line;
            // 首尾无换行
        }
    }
    return result;
}

// base
inline void printChar(const char &c) {
    std::cout << c;
}
inline void printString(const std::string &s) {
    std::cout << shift(s, genTabStr(depth + 1));
}
inline void printEndl() {
    std::cout << std::endl;
}

// declaration
template<typename T>
void printSeqHelp(const T &data, char leftBound, char rightBound);
template<typename T>
void printDictHelp(const T &data, char leftBound = '{', char rightBound = '}');

// to string
// #include "to_string_cpp17.h"
// #include "to_string_cpp11_gcc4.h"
// #include "to_string_cpp11_gcc12.h"
#include "to_string.h"

// print
template<class T, class = void>
struct StdOutput : std::false_type {};
template<class T>
struct StdOutput<
    T,
    decltype(std::declval<std::ostream &>() << std::declval<T>(), void())>
    : std::true_type {};

template<class T, class = void>
struct ToString : std::false_type {};
template<class T>
struct ToString<T, decltype(to_string(std::declval<T>()), void())>
    : std::true_type {};

template<class T, class = void>
struct ObjPrintMethod : std::false_type {};
template<class T>
struct ObjPrintMethod<T, decltype(std::declval<T>().ObjPrint(), void())>
    : std::true_type {};

template<class T, class = void>
struct PtrObjPrintMethod : std::false_type {};
template<class T>
struct PtrObjPrintMethod<T, decltype(std::declval<T>()->ObjPrint(), void())>
    : std::true_type {};

enum class PrintFlag {
    StdOutput,
    ToString,
    ObjPrintMethod,
    PtrObjPrintMethod,
    Error,
};

template<typename T>
constexpr PrintFlag get_flag() {
    return ObjPrintMethod<T>::value
               ? PrintFlag::ObjPrintMethod
               : (PtrObjPrintMethod<T>::value
                      ? PrintFlag::PtrObjPrintMethod
                      : (ToString<T>::value
                             ? PrintFlag::ToString
                             : (StdOutput<T>::value ? PrintFlag::StdOutput
                                                    : PrintFlag::Error)));
}

template<typename T>
typename std::enable_if<get_flag<T>() == PrintFlag::ObjPrintMethod>::type
printRecursion(const T &data) {
    data.ObjPrint();
}

template<typename T>
typename std::enable_if<get_flag<T>() == PrintFlag::PtrObjPrintMethod>::type
printRecursion(const T &data) {
    data->ObjPrint();
}

template<typename T>
typename std::enable_if<get_flag<T>() == PrintFlag::ToString>::type
printRecursion(const T &data) {
    printString(to_string(data));
}

template<typename T>
typename std::enable_if<get_flag<T>() == PrintFlag::StdOutput>::type
printRecursion(const T &data) {
    std::cout << data;
}

template<typename T, std::size_t N>
void printRecursion(const std::array<T, N> &data) {
    printSeqHelp(data, '[', ']');
    printChar('[');
    printRecursion(N);
    printChar(']');
}
template<typename T>
void printRecursion(const std::vector<T> &data) {
    printSeqHelp(data, '[', ']');
}
template<typename T>
void printRecursion(const std::set<T> &data) {
    printSeqHelp(data, '(', ')');
}
template<typename T>
void printRecursion(const std::unordered_set<T> &data) {
    printSeqHelp(data, '(', ')');
}

template<typename K, typename V>
void printRecursion(const std::map<K, V> &data) {
    printDictHelp(data);
}
template<typename K, typename V>
void printRecursion(const std::unordered_map<K, V> &data) {
    printDictHelp(data);
}
template<typename K, typename V>
void printRecursion(const std::pair<K, V> &data) {
    printChar('<');
    printRecursion(data.first);
    printChar(':');
    printRecursion(data.second);
    printChar('>');
}
template<typename T>
void printRecursion(std::priority_queue<T> priority_que) {
    std::vector<T> content;
    content.resize(priority_que.size());
    for (int i = 0; i < content.size(); ++i) {
        content[i] = std::move(priority_que.top());
        priority_que.pop();
    }
    assert(priority_que.empty());
    printSeqHelp(content, '>', ']');
}

template<typename Tuple, std::size_t N>
struct TuplePrinter {
    static void print(const Tuple &t) {
        TuplePrinter<Tuple, N - 1>::print(t);
        printChar(',');
        printRecursion(std::get<N - 1>(t));
    }
};
template<typename Tuple>
struct TuplePrinter<Tuple, 1> {
    static void print(const Tuple &t) { printRecursion(std::get<0>(t)); }
};
template<typename... Args>
void printRecursion(const std::tuple<Args...> &t) {
    printChar('(');
    TuplePrinter<decltype(t), sizeof...(Args)>::print(t);
    printChar(')');
}

#if __cplusplus >= 201703L

template<typename... Types>
void printRecursion(const std::nullopt_t &t) {
    printString("std::nullopt");
}

template<typename T>
void printRecursion(const std::optional<T> &t) {
    if (!t.has_value()) {
        printString("std::nullopt");
    } else {
        printRecursion(t.value());
    }
}

template<typename... Types>
void printRecursion(const std::monostate &t) {
    printString("std::monostate");
}

template<typename... Types>
void printRecursion(const std::variant<Types...> &t) {
    std::visit([](const auto &v) { printRecursion(v); }, t);
}

#endif

template<typename T>
void printSeqHelp(const T &data, char leftbound, char rightbound) {
    printChar(leftbound);
    if (data.empty()) {
        printString(" Empty ");
    } else {
        bool first = true;
        for (auto &ele : data) {
            if (!first) printChar(',');
            else
                first = false;
            printRecursion(ele);
        }
    }

    printChar(rightbound);
}

template<typename T>
void printDictHelp(const T &data, char leftbound, char rightbound) {
    ++depth;
    if (data.empty()) {
        printChar(leftbound);
        printString(" Empty ");
        printChar(rightbound);
    } else {
        printChar(leftbound);
        printEndl();
        for (auto &iter : data) {
            printString(genTabStr(depth + 1));
            printRecursion(iter.first);
            printChar(':');
            ++depth;
            printRecursion(iter.second);
            --depth;
            printEndl();
        }
        printString(genTabStr(depth));
        printChar(rightbound);
    }
    --depth;
}

} // namespace ObjPrint

#define PRINT(var) ObjPrint::printRecursion(var);

#define print(var)                  \
    do {                            \
        std::cout << #var << " = "; \
        PRINT(var)                  \
    } while (false);

#define PRINTLN(var)                   \
    do {                               \
        ObjPrint::printRecursion(var); \
        ObjPrint::printEndl();         \
    } while (false);

#define println(var)                \
    do {                            \
        std::cout << #var << " = "; \
        PRINTLN(var)                \
    } while (false);

// #define PRINTS(arg, ...)                                       \
//     do {                                                       \
//         PRINT(arg);                                            \
//         PRINT(", ");                                           \
//         if ((sizeof...(__VA_ARGS__)) > 0) PRINTS(__VA_ARGS__); \
//     } while (false);

// #define prints(arg, ...)                                       \
//     do {                                                       \
//         print(arg);                                            \
//         PRINT(", ");                                           \
//         if ((sizeof...(__VA_ARGS__)) > 0) prints(__VA_ARGS__); \
//     } while (false);

// #define printsln(arg, ...)                                       \
//     do {                                                         \
//         print(arg);                                              \
//         PRINT(", ");                                             \
//         if ((sizeof...(__VA_ARGS__)) > 0) printsln(__VA_ARGS__); \
//         ObjPrint::printEndl();                                   \
//     } while (false);
