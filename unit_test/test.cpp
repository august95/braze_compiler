#include "pch.h"

#include "../compiler_lib/compiler_lib.cpp"
#include "../compiler_lib/compilerProcess.h"
#include <string>

std::string file_path = "test_files";

TEST(lexer, number) {
  EXPECT_TRUE(true);


  compileProcess process;
  process.intialize("test_files/test_lexer.c");
  process.startCompiler();
}