#include "pch.h"
#include <string>
#include <list>
#include <iostream>
#include <sstream>
#include <fstream>

static bool compareLines(std::string file_name, std::string  asm_file) {
  std::istringstream stream1(file_name.c_str());
  std::istringstream stream2(asm_file.c_str());
  std::string line1, line2;
  int lineNumber = 1;
  bool areEqual = true;

  while (std::getline(stream1, line1) && std::getline(stream2, line2)) {
    if (stream1.eof() && !stream2.eof()) line1 = "";
    if (stream2.eof() && !stream1.eof()) line2 = "";

    if (line1 != line2) {
      std::cout << "Difference at line " << lineNumber << ":\n";
      std::cout << "  target   : \"" << line1 << "\"\n";
      std::cout << "  asm_file : \"" << line2 << "\"\n";
      areEqual = false;
    }
    lineNumber++;
  }

  return areEqual;
}



static bool compareFiles(std::string target, std::string asm_file)
{
  std::ifstream file;
  file.open(asm_file);
  if (!file) {
    std::cerr << "Failed to open the file.\n";
  }
  std::stringstream buffer;
  buffer << file.rdbuf(); // Read entire file at once

  return compareLines(target, buffer.str());
}