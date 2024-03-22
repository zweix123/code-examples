#include "Exception.h"
#include <gtest/gtest.h>

void throwAssertException() {
    throw AssertException("msg");
}

TEST(AssertException, general) {
    try {
        throwAssertException();
    } catch (const AssertException &e) { ASSERT_EQ(e.what(), "msg"); }
}