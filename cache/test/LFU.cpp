#include "LFU.h"
#include "obj_print.h"
#include <gtest/gtest.h>

TEST(LRU, leetcode1) {
    auto t = LFU<int, int>(2);
    t.put(1, 1);
    t.put(2, 2);
    const auto r1 = t.get(1);
    EXPECT_TRUE(r1.has_value());
    EXPECT_EQ(r1.value(), 1);
    t.put(3, 3);
    const auto r2 = t.get(2);
    EXPECT_TRUE(!r2.has_value());
    const auto r3 = t.get(3);
    EXPECT_TRUE(r3.has_value());
    EXPECT_EQ(r3.value(), 3);
    t.put(4, 4);
    const auto r4 = t.get(1);
    EXPECT_TRUE(!r4.has_value());
    const auto r5 = t.get(3);
    EXPECT_TRUE(r5.has_value());
    EXPECT_EQ(r5.value(), 3);
    const auto r6 = t.get(4);
    EXPECT_TRUE(r6.has_value());
    EXPECT_EQ(r6.value(), 4);
}

TEST(LRU, leetcode2) {
    auto t = LFU<int, int>(3);
    t.put(2, 2);
    t.put(1, 1);
    const auto r1 = t.get(2);
    EXPECT_TRUE(r1.has_value());
    EXPECT_EQ(r1.value(), 2);
    const auto r2 = t.get(1);
    EXPECT_TRUE(r2.has_value());
    EXPECT_EQ(r2.value(), 1);
    const auto r3 = t.get(2);
    EXPECT_TRUE(r3.has_value());
    EXPECT_EQ(r3.value(), 2);
    // println(t);
    t.put(3, 3);
    // println(t);
    t.put(4, 4);
    // println(t);
    const auto r4 = t.get(3);
    EXPECT_TRUE(!r4.has_value());
    const auto r5 = t.get(2);
    EXPECT_TRUE(r5.has_value());
    EXPECT_EQ(r5.value(), 2);
    const auto r6 = t.get(1);
    EXPECT_TRUE(r6.has_value());
    EXPECT_EQ(r6.value(), 1);
    const auto r7 = t.get(4);
    EXPECT_TRUE(r7.has_value());
    EXPECT_EQ(r7.value(), 4);
}
