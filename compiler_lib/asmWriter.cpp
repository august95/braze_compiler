#include "pch.h"
#include "asmWriter.h"
#include <stdarg.h>

asmWriter::asmWriter()
  : file(0)
{
}

void asmWriter::asmGen(std::string ins)
{

  fprintf(stdout, ins.c_str());
  fprintf(stdout, "\n");
  if (file)
  {
    fprintf(file, ins.c_str());
    fprintf(file, "\n");
  }
}

void asmWriter::asmGenArgs(const char* ins, va_list args)
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

void asmWriter::asmGenPushIns(std::string reg)
{
  m_stack_monitor.pushElement(PUSHED_VAUE);
  asmGen("push " + reg);
}

void asmWriter::asmGenPopIns(std::string reg)
{
  m_stack_monitor.popElement(PUSHED_VAUE);
  asmGen("pop " + reg);
}


void asmWriter::asmGenPushEbp(int stack_subtraction)
{
  m_stack_monitor.pushElement(BASE_POINTER);
  asmGen("push ebp");
  asmGen("mov ebp, esp");
  asmGen("sub esp, " + std::to_string(stack_subtraction));
}

void asmWriter::asmGenPopEbp(int stack_addition)
{
  m_stack_monitor.popElement(BASE_POINTER);
  asmGen("add esp, " + std::to_string(stack_addition));
  asmGen("pop ebp");
  asmGen("ret");
}

int asmWriter::initialize(std::string filename)
{
  m_filename = filename;

  return fopen_s(&file, filename.c_str(), "w");
}

void asmWriter::close()
{
    _fcloseall();
}


