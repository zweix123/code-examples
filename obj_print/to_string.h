#include <string>
#include <type_traits>

inline std::string to_string(char *s) {
    return s;
}

inline std::string to_string(std::string s) {
    return s;
}

inline std::string to_string(int val) {
    return std::to_string(val);
}
inline std::string to_string(unsigned val) {
    return std::to_string(val);
}
inline std::string to_string(long val) {
    return std::to_string(val);
}
inline std::string to_string(unsigned long val) {
    return std::to_string(val);
}
inline std::string to_string(long long val) {
    return std::to_string(val);
}
inline std::string to_string(unsigned long long val) {
    return std::to_string(val);
}
inline std::string to_string(float val) {
    return std::to_string(val);
}
inline std::string to_string(double val) {
    return std::to_string(val);
}
inline std::string to_string(long double val) {
    return std::to_string(val);
}
