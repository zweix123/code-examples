#include <exception>
#include <gtest/gtest.h>

void throw_exception_function() {
    throw std::exception();
}
void no_throw_exception_function() {
}
TEST(WarmUp, assert) {
    ASSERT_TRUE(true);
    ASSERT_FALSE(false);
    ASSERT_EQ(1, 1);
    ASSERT_NE(1, 2);
}
TEST(WarmUp, expect) {
    EXPECT_TRUE(true);
    EXPECT_FALSE(false);
    EXPECT_EQ(1, 1);
    EXPECT_NE(1, 2);
}
TEST(WarmUp, exception) {
    ASSERT_THROW(throw_exception_function(), std::exception);
    ASSERT_ANY_THROW(throw_exception_function());
    ASSERT_NO_THROW(no_throw_exception_function());
}
TEST(WarmUp, string) {
    ASSERT_STREQ("123", "123");
    ASSERT_STRNE("123", "456");
    std::string a = "123", b = "123";
    ASSERT_STREQ(a.c_str(), b.c_str());
    ASSERT_EQ(a, b);
}