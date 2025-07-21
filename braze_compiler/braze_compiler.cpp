// braze_compiler.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cctype>
#include <string>
#include "../compiler_lib/source/compiler_lib.cpp"
#include "../compiler_lib/compilerProcess.h"

int main(int argc, char* argv[]) {
  std::string input_file;
  std::string output_file;
  std::string output_bin;
  if (argc < 2) {
    std::cout << "No input file provided, using <project-root>/test_file.c\n";
    input_file = "./test_file.c";
    output_file = "./test_file.c.asm";
    output_bin = "./test_file";
  }
  else if (argc < 3)
  {
    input_file = argv[1];
    if(STRINGS_EQUAL(input_file.c_str(), "-h"))
    {
      //fixme: output help string to gui
    }
    else if(STRINGS_EQUAL(input_file.c_str(), "-help"))
    {
      //fixme: output help string to gui
    }
    else if(STRINGS_EQUAL(input_file.c_str(), "help"))
    {
      //fixme: output help string to gui
    }
  }

  std::string nasm_output_file = output_file + ".o";

  compileProcess process;
  process.initialize(input_file);
  
  int ret = process.startCompiler();
  if (ret != 0)
  {
    std::cout << "failed to compile " + input_file + "result: " + std::to_string(ret);
  }

#if defined(_MSC_VER)
  //Microsoft Visual Studio Compile
  std::cout << "\nno NASM support yet, compile with gcc: \n 1. make clean \n 2. make \n .3 ./bin/braze_compiler " ;
#else
    //GCC
  char nasm_cmd[512] = "nasm -f elf32 ./test_file.c.asm -o ./test_file.o && gcc -m32 ./test_file.o -o ./test_file -no-pie";
  //sprintf(nasm_cmd, "nasm -f elf32 %s -o %s", output_file, nasm_output_file);
  //sprintf(nasm_cmd, "nasm -f elf32 %s -o %s && gcc -m32 %s -o %s -no-pie", output_file.c_str(), nasm_output_file.c_str(), nasm_output_file.c_str(), output_file.c_str());
  //sprintf(nasm_cmd, "nasm -f elf32 %s -o %s", output_file, nasm_output_file);
  printf("%s", nasm_cmd);
  int res = system(nasm_cmd);
  if (res < 0)
  {
      return res;
  }  
  
#endif

}
