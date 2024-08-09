#include "pch.h"
#include "../../braze_compiler/braze_compiler/lexer.h"

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

TEST(braze_compiler, test)
{
  lexer lexer;
  EXPECT_TRUE(lexer.a == 5);
}