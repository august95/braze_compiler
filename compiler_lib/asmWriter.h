#pragma once
#include <cstdio> 
#include <string>
#include "stackMonitor.h"

class asmWriter
{
public:

  asmWriter();
  void asmGen(std::string ins);
  void asmGenArgs(const char* ins, va_list args);
  void asmGenPushIns(std::string reg);
  void asmGenPopIns(std::string reg);
  void asmGenPushEbp(int stack_subtraction);
  void asmGenPopEbp(int stack_addition);

  int initialize(std::string filename);
  void close();

  stackMonitor m_stack_monitor;
  std::string m_filename;
  FILE* file;

};

