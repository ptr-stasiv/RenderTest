#include "gtest/gtest.h"

#include <iostream>

#include "math/math.h"

TEST(VectorMath, Constructors)
{
   mm::vec3 v1;
   EXPECT_EQ(v1.x, 0);
   EXPECT_EQ(v1.y, 0);
   EXPECT_EQ(v1.z, 0);

   mm::vec3 v2 = { 5.0f, 1.0f, 3.0f };
   EXPECT_EQ(v2.x, 5.0f);
   EXPECT_EQ(v2.y, 1.0f);
   EXPECT_EQ(v2.z, 3.0f);

   mm::vec3 v3({ 5.0f, 1.0f, 3.0f });
   EXPECT_EQ(v3.x, 5.0f);
   EXPECT_EQ(v3.y, 1.0f);
   EXPECT_EQ(v3.z, 3.0f);

   mm::vec3 v4 = v3;
   EXPECT_EQ(v4.x, 5.0f);
   EXPECT_EQ(v4.y, 1.0f);
   EXPECT_EQ(v4.z, 3.0f);
}

TEST(VectorMath, Functions)
{
   mm::vec3 v1(2.0f, 3.0f, 0.0f);
   mm::vec3 v2(6.0f, 2.0f, 1.0f);

   EXPECT_EQ(mm::dot(v1, v2), 18.0f);

   mm::vec3 v3(3.0f, 0.0f, 0.0f);
   EXPECT_EQ(mm::length(v3), 3.0f);
}

int main(int argc, char* argv[])
{
   ::testing::InitGoogleTest(&argc, argv);

   RUN_ALL_TESTS();

   std::getchar();

   return -1;
}