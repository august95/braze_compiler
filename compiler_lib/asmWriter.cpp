#include "pch.h"
#include "asmWriter.h"
#include <stdarg.h>

asmWriter::asmWriter()
  : file(0)
{
}

void asmWriter::asmPush(std::string ins)
{

  fprintf(stdout, ins.c_str());
  fprintf(stdout, "\n");
  if (file)
  {
    fprintf(file, ins.c_str());
    fprintf(file, "\n");
  }
}

void asmWriter::asmPushArgs(const char* ins, va_list args)
{
  va_list args2;
  va_copy(args2, args);
  vfprintf(stdout, ins, args);
  fprintf(stdout, "\n");
  if (file)
  {
    vfprintf(file, ins, args2);
    fprintf(file, "\n");
  }
}

int asmWriter::initialize(std::string filename)
{
  m_filename = filename;

  return fopen_s(&file, filename.c_str(), "w");
}


