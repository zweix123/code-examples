#include "ObjPrint.h"
#include <gtest/gtest.h>

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
        testing::internal::CaptureStdout();
        int a = 1;
        long long b = 2;
        unsigned int c = 3;
        unsigned long long d = 4;
        float e = 5;
        double f = 6;
        uint8_t g = 7;
        std::string t = "1\n2\n3\n4\n5.000000\n6.000000\n7\n";
        ObjPrint::printExecute(a);
        ObjPrint::printEndl();
        ObjPrint::printExecute(b);
        ObjPrint::printEndl();
        ObjPrint::printExecute(c);
        ObjPrint::printEndl();
        ObjPrint::printExecute(d);
        ObjPrint::printEndl();
        ObjPrint::printExecute(e);
        ObjPrint::printEndl();
        ObjPrint::printExecute(f);
        ObjPrint::printEndl();
        ObjPrint::printExecute(g);
        ObjPrint::printEndl();
        std::string o = testing::internal::GetCapturedStdout();
        EXPECT_EQ(t, o);
    }
}
