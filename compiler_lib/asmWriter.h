#pragma once
#include <cstdio> 
#include <string>
class asmWriter
{
public:

  asmWriter();
  void asmPush(std::string ins);
  void asmPushArgs(const char* ins, va_list args);
  int initialize(std::string filename);

  std::string m_filename;
  FILE* file;

};

