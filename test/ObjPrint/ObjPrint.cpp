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
        // char *
        { // 正常
            testing::internal::CaptureStdout();
            std::string a = "Hello World";
            ObjPrint::printExecute(a.c_str());
            std::string b = testing::internal::GetCapturedStdout();
            EXPECT_EQ(a, b);
        }
        { // 超长/常
            testing::internal::CaptureStdout();
            std::string a = std::string(ObjPrint::CHARSTARTMAXLENGTH * 2, 'z');
            std::string b = std::string(ObjPrint::CHARSTARTMAXLENGTH, 'z');

            ObjPrint::printExecute(a.c_str());
            std::string c = testing::internal::GetCapturedStdout();
            EXPECT_EQ(b, c);
        }
    }
    { // std::string
        testing::internal::CaptureStdout();
        std::string a = "Hello World";
        ObjPrint::printExecute(a);
        std::string b = testing::internal::GetCapturedStdout();
        EXPECT_EQ(a, b);
    }
    { // number
        {
            testing::internal::CaptureStdout();
            int n = 42;
            ObjPrint::printExecute(n);
            std::string o = testing::internal::GetCapturedStdout();
            EXPECT_EQ(o, "42");
        }
        {
            testing::internal::CaptureStdout();
            int n = -42;
            ObjPrint::printExecute(n);
            std::string o = testing::internal::GetCapturedStdout();
            EXPECT_EQ(o, "-42");
        }
        {
            testing::internal::CaptureStdout();
            long n = 42;
            ObjPrint::printExecute(n);
            std::string o = testing::internal::GetCapturedStdout();
            EXPECT_EQ(o, "42");
        }
        {
            testing::internal::CaptureStdout();
            long long n = 42;
            ObjPrint::printExecute(n);
            std::string o = testing::internal::GetCapturedStdout();
            EXPECT_EQ(o, "42");
        }
        {
            testing::internal::CaptureStdout();
            unsigned n = 42;
            ObjPrint::printExecute(n);
            std::string o = testing::internal::GetCapturedStdout();
            EXPECT_EQ(o, "42");
        }
        {
            testing::internal::CaptureStdout();
            long long n = 42;
            ObjPrint::printExecute(n);
            std::string o = testing::internal::GetCapturedStdout();
            EXPECT_EQ(o, "42");
        }
        {
            testing::internal::CaptureStdout();
            double n = 3.14;
            ObjPrint::printExecute(n);
            std::string o = testing::internal::GetCapturedStdout();
            EXPECT_EQ(o, "3.140000");
        }
        {
            testing::internal::CaptureStdout();
            long double n = 3.14;
            ObjPrint::printExecute(n);
            std::string o = testing::internal::GetCapturedStdout();
            EXPECT_EQ(o, "3.140000");
        }
        {
            testing::internal::CaptureStdout();
            std::size_t n = 42;
            ObjPrint::printExecute(n);
            std::string o = testing::internal::GetCapturedStdout();
            EXPECT_EQ(o, "42");
        }
    }
}

#if defined(MultiMethod) || defined(DuplicateNameGetString)
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
#endif
#if defined(MultiMethod) || defined(DuplicateNameToString)
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
#endif

#ifdef DuplicateNameGetString
struct W3 {
    int a;
    double b;
    std::string c;
};

std::string getString(const W3 &w) {
    return "W3(" + std::to_string(w.a) + ", " + std::to_string(w.b) + ", " + w.c
           + ")";
}
#endif

#ifdef DuplicateNameToString
struct W4 {
    int a;
    double b;
    std::string c;
};

std::string toString(const W4 &w) {
    return "W4(" + std::to_string(w.a) + ", " + std::to_string(w.b) + ", " + w.c
           + ")";
}
#endif

TEST(ObjPrintTest, executeFuncCustomClass) {
#if defined(MultiMethod) || defined(DuplicateNameGetString)
    { // mothed getString in object
        testing::internal::CaptureStdout();
        auto t = W1(1, 2.0, "3");
        ObjPrint::printExecute(t);
        const auto o = testing::internal::GetCapturedStdout();
        EXPECT_EQ(o, "W1(1, 2.000000, 3)");
    }
#endif
#if defined(MultiMethod)
    { // mothed getString in pointer
        testing::internal::CaptureStdout();
        const auto p = std::make_shared<W1>(1, 2.0, "3");
        ObjPrint::printExecute(p);
        const auto o = testing::internal::GetCapturedStdout();
        EXPECT_EQ(o, "W1(1, 2.000000, 3)");
    }
#endif

#if defined(MultiMethod) || defined(DuplicateNameToString)

    { // mothed toString in object
        testing::internal::CaptureStdout();
        auto t = W2(1, 2.0, "3");
        ObjPrint::printExecute(t);
        const auto o = testing::internal::GetCapturedStdout();
        EXPECT_EQ(o, "W2(1, 2.000000, 3)");
    }
#endif
#if defined(MultiMethod)
    { // mothed toString in pointer
        testing::internal::CaptureStdout();
        const auto p = std::make_shared<W2>(1, 2.0, "3");
        ObjPrint::printExecute(p);
        const auto o = testing::internal::GetCapturedStdout();
        EXPECT_EQ(o, "W2(1, 2.000000, 3)");
    }
#endif

#ifdef DuplicateNameGetString
    { // function getString
        testing::internal::CaptureStdout();
        const auto t = W3{1, 2.0, "3"};
        ObjPrint::printExecute(t);
        const auto o = testing::internal::GetCapturedStdout();
        EXPECT_EQ(o, "W3(1, 2.000000, 3)");
    }
#endif
#ifdef DuplicateNameToString
    { // function toString
        testing::internal::CaptureStdout();
        const auto t = W4{1, 2.0, "3"};
        ObjPrint::printExecute(t);
        const auto o = testing::internal::GetCapturedStdout();
        EXPECT_EQ(o, "W4(1, 2.000000, 3)");
    }
#endif
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
    const std::string a = "ts = {\n  1: 1\n  2: 2\n  3: 3\n  4: 4\n}\n";
    testing::internal::CaptureStdout();
    std::map<int, int> ts{{1, 1}, {2, 2}, {3, 3}, {4, 4}};
    println(ts);
    std::string b = testing::internal::GetCapturedStdout();
    EXPECT_EQ(a, b);
}

TEST(ObjPrintTest, printlnUnorderedMap) {
    const std::string a = "ts = {\n  4: 4\n  3: 3\n  2: 2\n  1: 1\n}\n";
    testing::internal::CaptureStdout();
    std::unordered_map<int, int> ts{{1, 1}, {2, 2}, {3, 3}, {4, 4}};
    println(ts);
    std::string b = testing::internal::GetCapturedStdout();
    EXPECT_EQ(a, b);
}

TEST(ObjPrintTest, printlnTuple) {
    const std::string a = "t = (42, 3.140000, Hello World)\n";
    testing::internal::CaptureStdout();
    std::tuple<int, double, std::string> t{42, 3.14, "Hello World"};
    println(t);
    std::string b = testing::internal::GetCapturedStdout();
    EXPECT_EQ(a, b);
}
