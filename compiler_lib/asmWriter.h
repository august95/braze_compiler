#pragma once
#include <cstdio> 
#include <string>
#include "stackMonitor.h"
#include "datatype.h"

class asmWriter
{
public:

  asmWriter();
  void asmGen(std::string ins);
  void asmGenArgs(const char* ins, va_list args);
  void asmGenPushIns(std::string reg, std::shared_ptr < datatype > datatype, int offset_from_bp);
  void asmGenPopIns(std::string reg);
  void asmGenPushEbp(int stack_subtraction);
  void asmGenPopEbp(int stack_addition);
  int initialize(std::string filename);
  void close();
  std::shared_ptr <datatype> getDatatypeOnStack(int index = 0);

  stackMonitor m_stack_monitor;
  std::string m_filename;
  FILE* file;

};

