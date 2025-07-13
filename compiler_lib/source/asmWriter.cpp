#include "pch.h"
#include "../asmWriter.h"
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

void asmWriter::asmGenPushIns(std::string reg, std::shared_ptr < datatype > datatype, int offset_from_bp)
{
  m_stack_monitor.pushElement(PUSHED_VAUE, datatype, offset_from_bp);
  asmGen("push " + reg);
}

void asmWriter::asmGenPopIns(std::string reg)
{
  m_stack_monitor.popElement(PUSHED_VAUE);
  asmGen("pop " + reg);
}


void asmWriter::asmGenPushEbp(int stack_subtraction)
{
  m_stack_monitor.pushElement(BASE_POINTER,0,0);
  asmGen("push ebp");
  asmGen("mov ebp, esp");
  bool has_stack_size = stack_subtraction != 0;
  if (has_stack_size)
  {
    asmGen("sub esp, " + std::to_string(stack_subtraction));
  }
}

void asmWriter::asmGenPopEbp(int stack_addition)
{
  m_stack_monitor.popElement(BASE_POINTER);
  bool has_stack_size = stack_addition != 0;
  if (has_stack_size)
  {
    asmGen("add esp, " + std::to_string(stack_addition));
  }
  asmGen("pop ebp");
  asmGen("ret");
}

void asmWriter::asmGenNoNewLine(std::string ins)
{

  fprintf(stdout, ins.c_str());
  if (file)
  {
    fprintf(file, ins.c_str());
  }
}


void asmWriter::asmGenReduceRegister(std::string reg, int size, bool is_signed)
{
  if (size != DATA_SIZE_DWORD)
  {
    std::string ins = "movsx";
    if (!is_signed)
    {
      ins = "movzx";
    }

    asmGen(ins + " eax, " + getSubRegister(reg, size));
  }

}

int asmWriter::initialize(std::string filename)
{
  m_filename = filename;
  return fopen_s(&file, filename.c_str(), "w");
}

void asmWriter::discardUnusedStack()
{
  int stack_addition = m_stack_monitor.discardUnusedStack();
  if (stack_addition == 0)
  {
    return;
  }
  asmGen("add esp, " + std::to_string(stack_addition));
}

void asmWriter::close()
{
    _fcloseall();
}

std::shared_ptr <datatype> asmWriter::getDatatypeOnStack(int index)
{
  return m_stack_monitor.getDatatypeOnStack(index);
}

std::string asmWriter::getSubRegister(std::string reg, int size)
{
    std::string sub_register;
    if (STRINGS_EQUAL(reg.c_str(), "eax"))
    {
      if (size == DATA_SIZE_BYTE)
      {
        sub_register = "al";
      }
      else if (size == DATA_SIZE_WORD)
      {
        sub_register = "ax";
      }
      else if (size == DATA_SIZE_DWORD)
      {
        sub_register = "eax";
      }
    }
    else if (STRINGS_EQUAL(reg.c_str(), "ebx"))
    {
      if (size == DATA_SIZE_BYTE)
      {
        sub_register = "bl";
      }
      else if (size == DATA_SIZE_WORD)
      {
        sub_register = "bx";
      }
      else if (size == DATA_SIZE_DWORD)
      {
        sub_register = "ebx";
      }
    }
    else if (STRINGS_EQUAL(reg.c_str(), "ecx"))
    {
      if (size == DATA_SIZE_BYTE)
      {
        sub_register = "cl";
      }
      else if (size == DATA_SIZE_WORD)
      {
        sub_register = "cx";
      }
      else if (size == DATA_SIZE_DWORD)
      {
        sub_register = "ecx";
      }
    }
    else if (STRINGS_EQUAL(reg.c_str(), "edx"))
    {
      if (size == DATA_SIZE_BYTE)
      {
        sub_register = "dl";
      }
      else if (size == DATA_SIZE_WORD)
      {
        sub_register = "dx";
      }
      else if (size == DATA_SIZE_DWORD)
      {
        sub_register = "edx";
      }
    }

  return sub_register;
}


