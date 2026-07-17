// braze_compiler.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>
#include <cctype>
#include <string>
#include "../compiler_lib/braze_compiler.h"
#include "../compiler_lib/source/pch.h"
#include "../compiler_lib/compilerProcess.h"

void printHelpString();
void printBanner();
void printVersion();

int main(int argc, char* argv[]) {
  std::string input_file;
  std::string output_file;
  std::string output_bin;
  std::shared_ptr<std::stringstream> input_args = std::make_shared<std::stringstream>();
  bool input_file_mode = true;

  if (argc < 2) {
    std::cout << "\n-h -help or help for arg description\n";
    std::cout << "\nNo input file provided, using <project-root>/test_file.c\n";
#if _MSC_VER
    input_file = "./../test_file.c";
#else
    input_file = "./test_file.c";
#endif
    output_file = "./test_file.c.asm";
    output_bin = "./test_file";
  }
  else if (argc < 4)
  {
    if(STRINGS_EQUAL(argv[1], "--input_file"))
    {
      input_file = argv[2];
      output_file = input_file + ".asm";
      output_bin = input_file;
      const std::string suffix = ".c";
      output_bin.erase(input_file.size()-suffix.size());
    }

    if(STRINGS_EQUAL(argv[1], "-h") || STRINGS_EQUAL(argv[1], "-help") || STRINGS_EQUAL(argv[1], "help"))
    {
      printHelpString();
      return 0;
    }
    if(STRINGS_EQUAL(argv[1], "--input_code"))
    {
      *input_args << argv[2];
      output_file = "app.out.asm";
      output_bin = "app.out";
      const std::string suffix = ".c";
      input_file_mode = false;
    }

    if(STRINGS_EQUAL(argv[1], "-h") || STRINGS_EQUAL(argv[1], "-help") || STRINGS_EQUAL(argv[1], "help"))
    {
      printHelpString();
      return 0;
    }
    if (STRINGS_EQUAL(argv[1], "-v") || STRINGS_EQUAL(argv[1], "-version") || STRINGS_EQUAL(argv[1], "version"))
    {
      printVersion();
      return 0;
    }
    if (STRINGS_EQUAL(argv[1], "-braze"))
    {
      printBanner(); 
      printVersion();
      return 0;
    }
  }

  //std::cout <<"\ncompiling " << input_file << " to " << output_bin << "\n\n";

  std::string nasm_output_file = output_bin + ".o";

  //avoid libc debugging, allocato on heap
  compileProcess* process = new compileProcess();
  if(input_file_mode)
    process->initialize(input_file);
  else
    process->initialize(input_args);
  
  
  int ret = process->startCompiler();
  if (ret != 0)
  {
    std::cout << "failed to compile " + input_file + "result: " + std::to_string(ret);
  }
  delete process;

  if(input_file_mode)
  {   
      //nasm -f elf32 ./test_file.c.asm -o ./test_file.o && gcc -m32 ./test_file.o -o ./test_file -no-pie
      std::string nasm_cmd = "nasm -f elf32 ./" + output_file + " -o ./" + nasm_output_file + " && gcc -m32 ./" + nasm_output_file + " -o ./" + output_bin +" -no-pie";
    
      int res = system(nasm_cmd.c_str());
      if (res != 0)
      {
        std::cout << "\n failed to assemble: " << output_file << " with nasm!";
         return res;
      }  
  }
  
//#endif

}


void printHelpString()
{
  printBanner();
  printVersion();
  std::cout << "\nCompile file with braze:"; 
  std::cout << "\n  ./braze --input_file <file_to_compile>.c"
               "\n";
  std::cout << "\nCompile source through terminal:";
  std::cout << "\n  ./braze --input_code 'int main {...}'"
               "\n";
  std::cout << "\nBraze will compile <project-root>/test_file.c if no args are provided"
               "\n";
  std::cout << "\nOuput executable binary is named: <file_to_compile> (no suffix) will be created "
               "\n";
  std::cout << "\nbraze are generating *.asm files, that are assembled by nasm into *.o files";
  std::cout << "\nin command mode, no output binary are created. Assembly code is written to cout! \n";
}

void printBanner()
{
      std::cout <<
      "\noooooooooo.  ooooooooo.         .o.        oooooooooooo oooooooooooo     "
      "\n`888'   `Y8b `888   `Y88.      .888.      d'''''''d888' `888'     `8     "
      "\n 888     888  888   .d88'     .8'888.           .888P    888             "
      "\n 888oooo888'  888ooo88P'     .8' `888.         d888'     888oooo8        "
      "\n 888    `88b  888`88b.      .88ooo8888.      .888P       888    '        "
      "\n 888    .88P  888  '88b.   .8'     `888.    d888'    .P  888       o .o. "
      "\no888bood8P'  o888o  o888o o88o     o8888o .8888888888P  o888ooooood8 Y8P "
      "\n\n";
}

void printVersion()
{
  std::cout << "braze compiler " << BRAZE_VERSION << "\n";
}