// braze_compiler.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cctype>
#include <string>
#include "../compiler_lib/source/compiler_lib.cpp"
#include "../compiler_lib/compilerProcess.h"

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cout << "No input file provided!\n";
    return 1;
  }

  std::string input = argv[1];

	compileProcess process;
	process.initialize(input);

	int ret = process.startCompiler();
  if (ret != 0)
  {
    std::cout << "failed to compile " + input + "result: " + std::to_string(ret);
  }


}
