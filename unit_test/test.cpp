#include "pch.h"

#include "../compiler_lib/compiler_lib.cpp"
#include "../compiler_lib/compilerProcess.h"

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);


  compileProcess process;
  process.intialize();
  process.startCompiler();
}