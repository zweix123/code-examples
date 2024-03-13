#pragma once

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>

#include <map>
#include <queue>
#include <set>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace ObjPrint {

using std::string;
using std::vector;

constexpr int CHARSTARTMAXLENGTH = 120 + 7;

static int depth = -1;

// helper
inline string genTabStr(int tabNum) {
    return string(tabNum << 1, ' ');
}

inline std::string shift(const std::string &s, const std::string &pre) {
    std::stringstream ss(s);
    vector<string> lines;
    {
        string line;
        while (std::getline(ss, line)) { lines.push_back(std::move(line)); }
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
inline void printString(const std::string &s) {
    std::cout << shift(s, genTabStr(depth + 1));
}
inline void printEndl() {
    std::cout << std::endl;
}

// execute
inline void printExecute(const char *const s) {
    int endIdx = 0;
    for (; endIdx < CHARSTARTMAXLENGTH && s[endIdx]; ++endIdx)
        ;
    printString(string(s, endIdx));
}
template<typename T>
typename std::enable_if<std::is_same<T, std::string>::value>::type
printExecute(const T &str) {
    printString(str);
}
template<typename T>
typename std::enable_if<std::is_arithmetic<T>::value>::type
printExecute(const T &num) {
    printString(std::to_string(num));
}

template<typename T>
auto printExecute(const T &data) -> decltype(data.getString(), void()) {
    printString(data.getString());
}
template<typename T>
auto printExecute(const T &data) -> decltype(data->getString(), void()) {
    printString(data->getString());
}

template<typename T>
auto printExecute(const T &data) -> decltype(data.toString(), void()) {
    printString(data.toString());
}
template<typename T>
auto printExecute(const T &data) -> decltype(data->toString(), void()) {
    printString(data->toString());
}

// declaration
template<typename T>
void printSeqHelp(const T &data, char leftBound, char rightBound);
template<typename T>
void printDictHelp(const T &data, char leftBound = '{', char rightBound = '}');
template<typename T>
void printRecursion(const T &data);

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
    printString("<");
    printRecursion(data.first);
    printString(":");
    printRecursion(data.second);
    printString(">");
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
        printString(", ");
        printRecursion(std::get<N - 1>(t));
    }
};
template<typename Tuple>
struct TuplePrinter<Tuple, 1> {
    static void print(const Tuple &t) { printRecursion(std::get<0>(t)); }
};
template<typename... Args>
void printRecursion(const std::tuple<Args...> &t) {
    std::cout << "(";
    TuplePrinter<decltype(t), sizeof...(Args)>::print(t);
    std::cout << ")";
}

template<typename T>
void printRecursion(const T &data) {
    printExecute(data);
}

template<typename T>
void printSeqHelp(const T &data, char leftbound, char rightbound) {
    std::cout << leftbound;
    if (data.empty()) {
        printString(" Empty ");
    } else {
        bool first = true;
        for (auto &ele : data) {
            if (!first) printString(",");
            else
                first = false;
            printRecursion(ele);
        }
    }
    std::cout << rightbound;
}

template<typename T>
void printDictHelp(const T &data, char leftbound, char rightbound) {
    ++depth;
    if (data.empty()) {
        std::cout << leftbound;
        printString(" Empty ");
        std::cout << rightbound;
    } else {
        std::cout << leftbound << "\n";
        for (auto &iter : data) {
            printString(genTabStr(depth + 1));
            printRecursion(iter.first);
            printString(": ");
            ++depth;
            printRecursion(iter.second);
            --depth;
            printEndl();
        }
        std::cout << genTabStr(depth) << rightbound;
    }
    --depth;
}

} // namespace ObjPrint

// template<typename T>
// void println(const std::string &var, const T &data) {
//     printString(var);
//     printString("= ");
//     printRecursion(data);
//     printEndl();
// }

// } // namespace ObjPrint

// #define cicada(var)
//     do {
//         std::cout << #var << " = ";
//         ObjPrint::printRecursion(var);
//         ObjPrint::printEndl();
//     } while (false);