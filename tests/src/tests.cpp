#include "gtest/gtest.h"

float bar()
{
   return -1.0f;
}

TEST(TestSuit, Test)
{
   EXPECT_EQ(bar(), 0);
}

int main(int argc, char* argv[])
{
   ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}