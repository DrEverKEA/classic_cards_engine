#include <gtest/gtest.h>

// A simple sanity check to ensure the testing framework compiles and runs
TEST(SanityCheck, BuildSystemWorks) {
    EXPECT_EQ(1 + 1, 2);
    EXPECT_TRUE(true);
}
