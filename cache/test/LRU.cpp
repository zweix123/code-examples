#include "LRU.h"
#include "obj_print.h"
#include <gtest/gtest.h>

void createInstance() {
    const auto t = LRU<int, int>(0);
}

TEST(LRU, exception) {
    ASSERT_THROW(createInstance(), AssertException);
}

TEST(LRU, leetcode) {
    auto t = LRU<int, int>(2);
    EXPECT_FALSE(t.get(2).has_value());
    t.put(2, 6);
    // println(t);
    EXPECT_FALSE(t.get(1).has_value());
    t.put(1, 5);
    // println(t);
    t.put(1, 2);
    // println(t);
    const auto r1 = t.get(1);
    EXPECT_TRUE(r1.has_value());
    EXPECT_EQ(r1.value(), 2);
    const auto r2 = t.get(2);
    EXPECT_TRUE(r2.has_value());
    EXPECT_EQ(r2.value(), 6);
}
