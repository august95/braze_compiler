#pragma once
#include <cstdio>
#include <string>
#include "stackMonitor.h"
#include "../datatype.h"
#include <memory>

class asmWriter
{
  public:
  enum WriteMode
  {
    W_FILE,
    W_STDOUT  
  };
  asmWriter();
  void asmGen(std::string ins);
  void asmGenArgs(const char *ins, va_list args);
  void asmGenPushIns(std::string reg, std::shared_ptr<datatype> datatype, int offset_from_bp);
  void asmGenPopIns(std::string reg);
  void asmGenPushEbp(int stack_subtraction);
  void asmGenPopEbp(int stack_addition);
  void asmGenNoNewLine(std::string ins);
  void asmGenReduceRegister(std::string reg, int size, bool is_signed = false);
  int initialize(std::string filename);
  void discardUnusedStack();
  void addStack(int stack_size);
  void close();
  void setWriteMode(WriteMode mode){m_mode = mode;}
  std::shared_ptr<datatype> getDatatypeOnStack(int index = 0);

private:
  std::string getSubRegister(std::string reg, int size);

  WriteMode m_mode;
  stackMonitor m_stack_monitor;
  std::string m_filename;
  FILE *file;
};
