#include "ObjPrint.h"

#include <gtest/gtest.h>

#include <array>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

TEST(ObjPrintTest, genTabStrTest) {
    EXPECT_EQ("", ObjPrint::genTabStr(0));
    std::string a;
    for (int i = 1; i < 10; ++i) {
        a += "  ";
        std::string b = ObjPrint::genTabStr(i);
        EXPECT_EQ(a, b);
    }
}

TEST(ObjPrintTest, shiftTest) {
    {
        // 首无换行
        std::string a = "Hello World";
        std::string p = "  ";
        std::string b = "Hello World";
        EXPECT_EQ(ObjPrint::shift(a, p), b);
    }
    {
        // 尾无换行
        std::string a = "Hello World\nHello World";
        std::string p = "  ";
        std::string b = "Hello World\n  Hello World";
        EXPECT_EQ(ObjPrint::shift(a, p), b);
    }
    {
        std::string a = "Hello World\nHello World\n  Hello World";
        std::string p = "  ";
        std::string b = "Hello World\n  Hello World\n    Hello World";
        EXPECT_EQ(ObjPrint::shift(a, p), b);
    }
}

TEST(ObjPrintTest, baseFunc) {
    {
        testing::internal::CaptureStdout();
        std::string a = "Hello World";
        ObjPrint::printString(a);
        std::string b = testing::internal::GetCapturedStdout();
        EXPECT_EQ(a, b);
    }
    {
        testing::internal::CaptureStdout();
        ObjPrint::printEndl();
        std::string o = testing::internal::GetCapturedStdout();
        EXPECT_EQ(o, "\n");
    }
}
TEST(ObjPrintTest, executeFunc) {
    {
        // char*
        { // 字面量
            const char *s1 = "Hello World";
            const char *const s2 = "Hello World";
            EXPECT_EQ(ObjPrint::to_string(s1), s1);
            EXPECT_EQ(ObjPrint::to_string(s2), s2);
        }
        { // std::string().c_str()
            std::string s1 = "Hello World";
            const std::string s2 = "Hello World";
            EXPECT_EQ(ObjPrint::to_string(s1.c_str()), s1);
            EXPECT_EQ(ObjPrint::to_string(s2.c_str()), s2);
        }
        {
            std::string a = std::string(120 * 2, 'z');
            std::string b = std::string(120, 'z');
            std::string c = ObjPrint::to_string(a.c_str());
            EXPECT_EQ(b, c);
        }
    }

    { // std::string
        std::string s = "Hello World";
        EXPECT_EQ(s, ObjPrint::to_string(s));
    }
    { // number
        {
            int n = 42;
            EXPECT_EQ(ObjPrint::to_string(n), "42");
        }
        {
            int n = -42;
            EXPECT_EQ(ObjPrint::to_string(n), "-42");
        }
        {
            long n = 42;
            EXPECT_EQ(ObjPrint::to_string(n), "42");
        }
        {
            long long n = 42;
            EXPECT_EQ(ObjPrint::to_string(n), "42");
        }
        {
            unsigned n = 42;
            EXPECT_EQ(ObjPrint::to_string(n), "42");
        }
        {
            long long n = 42;
            EXPECT_EQ(ObjPrint::to_string(n), "42");
        }
        {
            double n = 3.14;
            EXPECT_EQ(ObjPrint::to_string(n), "3.140000");
        }
        {
            long double n = 3.14;
            EXPECT_EQ(ObjPrint::to_string(n), "3.140000");
        }
        {
            std::size_t n = 42;
            EXPECT_EQ(ObjPrint::to_string(n), "42");
        }
    }
}

class W1 {
    int a;
    double b;
    std::string c;

  public:
    W1() = delete;
    W1(int m_a, double m_b, std::string m_c) : a{m_a}, b{m_b}, c{m_c} {};
    std::string getString() const {
        return "W1(" + std::to_string(a) + ", " + std::to_string(b) + ", " + c
               + ")";
    }
};

class W2 {
    int a;
    double b;
    std::string c;

  public:
    W2() = delete;
    W2(int m_a, double m_b, std::string m_c) : a{m_a}, b{m_b}, c{m_c} {};
    std::string toString() const {
        return "W2(" + std::to_string(a) + ", " + std::to_string(b) + ", " + c
               + ")";
    }
};

struct W3 {
    int a;
    double b;
    std::string c;
};

std::string getString(const W3 &w) {
    return "W3(" + std::to_string(w.a) + ", " + std::to_string(w.b) + ", " + w.c
           + ")";
}

struct W4 {
    int a;
    double b;
    std::string c;
};

std::string toString(const W4 &w) {
    return "W4(" + std::to_string(w.a) + ", " + std::to_string(w.b) + ", " + w.c
           + ")";
}

TEST(ObjPrintTest, executeFuncCustomClass) {
    { // mothed getString in object
        auto t = W1(1, 2.0, "3");
        EXPECT_EQ(ObjPrint::to_string(t), "W1(1, 2.000000, 3)");
    }
    { // mothed getString in pointer
        const auto p = std::make_shared<W1>(1, 2.0, "3");
        EXPECT_EQ(ObjPrint::to_string(p), "W1(1, 2.000000, 3)");
    }
    { // mothed toString in object
        auto t = W2(1, 2.0, "3");
        EXPECT_EQ(ObjPrint::to_string(t), "W2(1, 2.000000, 3)");
    }
    { // mothed toString in pointer
        const auto p = std::make_shared<W2>(1, 2.0, "3");
        EXPECT_EQ(ObjPrint::to_string(p), "W2(1, 2.000000, 3)");
    }
    { // function getString
        const auto t = W3{1, 2.0, "3"};
        EXPECT_EQ(ObjPrint::to_string(t), "W3(1, 2.000000, 3)");
    }
    { // function toString
        const auto t = W4{1, 2.0, "3"};
        EXPECT_EQ(ObjPrint::to_string(t), "W4(1, 2.000000, 3)");
    }
}

TEST(ObjPrintTest, printlnBase) {
    {
        testing::internal::CaptureStdout();
        int n = 42;
        println(n);
        std::string s = testing::internal::GetCapturedStdout();
        EXPECT_EQ(s, "n = 42\n");
    }
}

TEST(ObjPrintTest, printlnVector) {
    const std::string a = "vec = [1,2,3,4]\n";
    testing::internal::CaptureStdout();
    std::vector<int> vec{1, 2, 3, 4};
    println(vec);
    std::string b = testing::internal::GetCapturedStdout();
    EXPECT_EQ(a, b);
}

TEST(ObjPrintTest, printlnArray) {
    const std::string a = "arr = [1,2,3,4][4]\n";
    testing::internal::CaptureStdout();
    std::array<int, 4> arr{1, 2, 3, 4};
    println(arr);
    std::string b = testing::internal::GetCapturedStdout();
    EXPECT_EQ(a, b);
}
TEST(ObjPrintTest, printlnSet) {
    const std::string a = "ts = (1,2,3,4)\n";
    testing::internal::CaptureStdout();
    std::set<int> ts{1, 2, 3, 4};
    println(ts);
    std::string b = testing::internal::GetCapturedStdout();
    EXPECT_EQ(a, b);
}

TEST(ObjPrintTest, printlnUnorderedSet) {
    const std::string a = "ts = (4,3,2,1)\n";
    testing::internal::CaptureStdout();
    std::unordered_set<int> ts{1, 2, 3, 4};
    println(ts);
    std::string b = testing::internal::GetCapturedStdout();
    EXPECT_EQ(a, b);
}

TEST(ObjPrintTest, printlnMap) {
    const std::string a = "ts = {\n  1:1\n  2:2\n  3:3\n  4:4\n}\n";
    testing::internal::CaptureStdout();
    std::map<int, int> ts{{1, 1}, {2, 2}, {3, 3}, {4, 4}};
    println(ts);
    std::string b = testing::internal::GetCapturedStdout();
    EXPECT_EQ(a, b);
}

TEST(ObjPrintTest, printlnUnorderedMap) {
    const std::string a = "ts = {\n  4:4\n  3:3\n  2:2\n  1:1\n}\n";
    testing::internal::CaptureStdout();
    std::unordered_map<int, int> ts{{1, 1}, {2, 2}, {3, 3}, {4, 4}};
    println(ts);
    std::string b = testing::internal::GetCapturedStdout();
    EXPECT_EQ(a, b);
}

TEST(ObjPrintTest, printlnTuple) {
    const std::string a = "t = (42,3.140000,Hello World)\n";
    testing::internal::CaptureStdout();
    std::tuple<int, double, std::string> t{42, 3.14, "Hello World"};
    println(t);
    std::string b = testing::internal::GetCapturedStdout();
    EXPECT_EQ(a, b);
}

struct W5 {
    int a;
    double b;
    std::string c;
    void ObjPrint() const {
        std::cout << "a = " << a << ", b = " << b << ", c = " << c;
    }
};

struct W6 {
    int a;
    double b;
    std::string c;
    friend std::ostream &operator<<(std::ostream &out, const W6 &w) {
        return out << "a = " << w.a << ", b = " << w.b << ", c = " << w.c;
    }
};

TEST(ObjPrintTest, printClass) {
    { // method ObjPrint
        testing::internal::CaptureStdout();
        const auto t = W5{1, 2.0, "3"};
        println(t);
        std::string o = testing::internal::GetCapturedStdout();
        EXPECT_EQ(o, "t = a = 1, b = 2, c = 3\n");
    }
    { // std::cout
        testing::internal::CaptureStdout();
        const auto t = W6{1, 2.0, "3"};
        println(t);
        std::string o = testing::internal::GetCapturedStdout();
        EXPECT_EQ(o, "t = a = 1, b = 2, c = 3\n");
        // ToString<W6>::value
    }
}

TEST(ObjPrintTest, printlnVariant) {
    using VT = std::variant<std::monostate, int, double, std::string>;

    {
        testing::internal::CaptureStdout();
        VT z = std::monostate{};
        println(z);
        std::string o = testing::internal::GetCapturedStdout();
        EXPECT_EQ(o, "z = std::monostate\n");
    }
    {
        testing::internal::CaptureStdout();
        VT a = 1;
        println(a);
        std::string o = testing::internal::GetCapturedStdout();
        EXPECT_EQ(o, "a = 1\n");
    }
    {
        testing::internal::CaptureStdout();
        VT b = 3.14;
        println(b);
        std::string o = testing::internal::GetCapturedStdout();
        EXPECT_EQ(o, "b = 3.140000\n");
    }
    {
        testing::internal::CaptureStdout();
        VT c = "Hello World";
        println(c);
        std::string o = testing::internal::GetCapturedStdout();
        EXPECT_EQ(o, "c = Hello World\n");
    }
}

TEST(ObjPrintTest, MultiPrint) {
    const std::vector<int> t{1, 2, 3};
    PRINT(t);
    print(t);
    PRINTLN(t);
    println(t);
    // PRINTS(t[0], t[1], t[2]);
    // prints(t[0], t[1], t[2]);
    // PRINTSLN(t[0], t[1], t[2]);
    // printsln(t[0], t[1], t[2]);
}