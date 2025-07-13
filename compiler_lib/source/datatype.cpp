#include "pch.h"
#include "../datatype.h"
#include <assert.h>
#include "../braze_compiler.h"

datatype::datatype()

  :m_first(primitiveType::DATA_TYPE_NONE),
  m_second(primitiveType::DATA_TYPE_NONE),
  m_unsigned(0),
  m_signed(0),
  m_static(0),
  m_const(0),
  m_extern(0),
  m_has_secondary_primitive_type(0),
  m_pointer_depth(0),
  m_datatype_size(0),
  m_r_value(0)
{
}


datatype::datatype(filePosition file_position)
  :m_first(primitiveType::DATA_TYPE_NONE),
  m_second(primitiveType::DATA_TYPE_NONE),
  m_unsigned(0),
  m_signed(0),
  m_static(0),
  m_const(0),
  m_extern(0),
  m_has_secondary_primitive_type(0),
  m_pointer_depth(0),
  m_file_position(file_position), 
  m_datatype_size(0),
  m_r_value(0)
{
}

void datatype::setKeyVariableModifier(std::string modifier)
{
  if (STRINGS_EQUAL(modifier.c_str(), "unsigned"))
  {
    m_unsigned = true;
    return;
  }
  else if (STRINGS_EQUAL(modifier.c_str(), "signed"))
  {
    m_signed = true;
    return;
  }
  else if (STRINGS_EQUAL(modifier.c_str(), "static"))
  {
    m_static = true;
    return;
  }
  else if (STRINGS_EQUAL(modifier.c_str(), "const"))
  {
    m_const = true;
    return;
  }
  else if (STRINGS_EQUAL(modifier.c_str(), "extern"))
  {
    m_extern = true;
    return;
  }
  else
  {
    cerror("expected variable modifier");
    assert(false);
  }

}

void datatype::setDataType(std::string data_type)
{
  primitiveType primitive_type = primitiveType::DATA_TYPE_NONE;
  if (STRINGS_EQUAL(data_type.c_str(), "void"))
  {
    primitive_type = primitiveType::DATA_TYPE_VOID;
  }
  else if (STRINGS_EQUAL(data_type.c_str(), "char"))
  {
    primitive_type = primitiveType::DATA_TYPE_CHAR;
    m_unsigned = true;
    m_signed = false;
  }
  else if (STRINGS_EQUAL(data_type.c_str(), "int"))
  {
    primitive_type = primitiveType::DATA_TYPE_INTEGER;
  }
  else if (STRINGS_EQUAL(data_type.c_str(), "short"))
  {
    primitive_type = primitiveType::DATA_TYPE_SHORT;
  }
  else if (STRINGS_EQUAL(data_type.c_str(), "float"))
  {
    primitive_type = primitiveType::DATA_TYPE_FLOAT;
  }
  else if (STRINGS_EQUAL(data_type.c_str(), "double"))
  {
    primitive_type = primitiveType::DATA_TYPE_DOUBLE;
  }
  else if (STRINGS_EQUAL(data_type.c_str(), "long"))
  {
    primitive_type = primitiveType::DATA_TYPE_LONG;
  }
  else if (STRINGS_EQUAL(data_type.c_str(), "struct"))
  {
    primitive_type = primitiveType::DATA_TYPE_STRUCT;
  }
  else if (STRINGS_EQUAL(data_type.c_str(), "union"))
  {
    primitive_type = primitiveType::DATA_TYPE_UNION;
  }
  else if (STRINGS_EQUAL(data_type.c_str(), "bool"))
  {
    primitive_type = primitiveType::DATA_TYPE_BOOL;
  }
  else if (STRINGS_EQUAL(data_type.c_str(), "__internal_only_string__"))
  {
    // R value string dont have any datatype delcaration:
    //function_call("r_value_string");
    primitive_type = primitiveType::DATA_TYPE_CHAR;
    incrementPointerDepth();
    m_const = true;
    m_unsigned = true;
    m_signed = false;
  }
  else
  {
    cerror("expected primitive data type");
    assert(false);
  }
  if (m_first == primitiveType::DATA_TYPE_NONE)
  {
    m_first = primitive_type;
    calcualteDatatypeSize();
    return;
  }
  m_second = primitive_type;
  m_has_secondary_primitive_type = true;
  calcualteDatatypeSize();

  //FIXME: perform validation?

}

void datatype::incrementPointerDepth()
{ 
  m_pointer_depth++; 
  calcualteDatatypeSize();
}

void datatype::calcualteDatatypeSize()
{
  m_datatype_size = getPrimitiveTypeSize(m_first);
  if (m_has_secondary_primitive_type)
  {
    m_datatype_size += getPrimitiveTypeSize(m_second);
  }
  if (m_pointer_depth > 0)
  {
    m_datatype_size = 4;  //Fixme: for now 32 bit architecture, pointer size is 4 bytes
  }
}

int datatype::getPrimitiveTypeSize(primitiveType primitive_type)
{
  int size = 0;

  switch (primitive_type)
  {
  case primitiveType::DATA_TYPE_NONE:
    size = DATA_SIZE_NONE;
    break;

  case primitiveType::DATA_TYPE_VOID:
    size = DATA_SIZE_NONE;
    break;

  case primitiveType::DATA_TYPE_CHAR:
    size = DATA_SIZE_BYTE;
    break;

  case primitiveType::DATA_TYPE_SHORT:
    size = DATA_SIZE_WORD;
    break;

  case primitiveType::DATA_TYPE_INTEGER:
    size = DATA_SIZE_DWORD;
    break;

  case primitiveType::DATA_TYPE_LONG:
    size = DATA_SIZE_DWORD;
    break;

  case primitiveType::DATA_TYPE_FLOAT:
    size = DATA_SIZE_DWORD;
    break;

  case primitiveType::DATA_TYPE_DOUBLE:
    size = DATA_SIZE_DWORD;
    break;
  case primitiveType::DATA_TYPE_BOOL:
    size = DATA_SIZE_BYTE;
    break;

  case primitiveType::DATA_TYPE_UNKNOWN:
    size = DATA_SIZE_DDWORD;
    break;
  
  }
  return size;
}

std::string datatype::getStringForPrimitiveSize()
{
  if (m_datatype_size == DATA_SIZE_BYTE)
  {
    return "db";
  }
  else if (m_datatype_size == DATA_SIZE_WORD)
  {
    return "dm";
  }
  else if (m_datatype_size == DATA_SIZE_DWORD)
  {
    return "dd";
  }
  else if (m_datatype_size == DATA_SIZE_DDWORD)
  {
    return "dq";
  }
  cerror("Codegen: generating global variable with no size", m_file_position);
  assert(0);
}

std::string datatype::getDatatypeRegisterSize()
{
  if (m_datatype_size == DATA_SIZE_BYTE)
  {
    return "byte";
  }
  else if (m_datatype_size == DATA_SIZE_WORD)
  {
    return "word";
  }
  else if (m_datatype_size == DATA_SIZE_DWORD)
  {
    return "dword";
  }
  else if (m_datatype_size == DATA_SIZE_DDWORD)
  {
    return "ddword";
  }
}

void datatype::getRegToUse(std::string& reg_to_use)
{
  if (STRINGS_EQUAL(reg_to_use.c_str(), "eax"))
  {
    if (m_datatype_size == DATA_SIZE_BYTE)
    {
      reg_to_use = "al";
    }
    else if (m_datatype_size == DATA_SIZE_WORD)
    {
      reg_to_use = "ax";
    }
    else if (m_datatype_size == DATA_SIZE_DWORD)
    {
      reg_to_use = "eax";
    }
  }
  else if (STRINGS_EQUAL(reg_to_use.c_str(), "ebx"))
  {
    if (m_datatype_size == DATA_SIZE_BYTE)
    {
      reg_to_use = "bl";
    }
    else if (m_datatype_size == DATA_SIZE_WORD)
    {
      reg_to_use = "bx";
    }
    else if (m_datatype_size == DATA_SIZE_DWORD)
    {
      reg_to_use = "ebx";
    }
  }
  else if (STRINGS_EQUAL(reg_to_use.c_str(), "ecx"))
  {
    if (m_datatype_size == DATA_SIZE_BYTE)
    {
      reg_to_use = "cl";
    }
    else if (m_datatype_size == DATA_SIZE_WORD)
    {
      reg_to_use = "cx";
    }
    else if (m_datatype_size == DATA_SIZE_DWORD)
    {
      reg_to_use = "ecx";
    }
  }
  else if (STRINGS_EQUAL(reg_to_use.c_str(), "edx"))
  {
    if (m_datatype_size == DATA_SIZE_BYTE)
    {
      reg_to_use = "dl";
    }
    else if (m_datatype_size == DATA_SIZE_WORD)
    {
      reg_to_use = "dx";
    }
    else if (m_datatype_size == DATA_SIZE_DWORD)
    {
      reg_to_use = "edx";
    }
  }

}


bool datatype::IsKeywordDatatype(std::string val)
{
  return STRINGS_EQUAL(val.c_str(), "void") ||
    STRINGS_EQUAL(val.c_str(), "char") ||
    STRINGS_EQUAL(val.c_str(), "int") ||
    STRINGS_EQUAL(val.c_str(), "short") ||
    STRINGS_EQUAL(val.c_str(), "bool") ||
    STRINGS_EQUAL(val.c_str(), "float") ||
    STRINGS_EQUAL(val.c_str(), "double") ||
    STRINGS_EQUAL(val.c_str(), "long") ||
    STRINGS_EQUAL(val.c_str(), "struct") ||
    STRINGS_EQUAL(val.c_str(), "union");
}


bool datatype::isKeywordVariableModifier(std::string val)
{
  return STRINGS_EQUAL(val.c_str(), "unsigned") ||
    STRINGS_EQUAL(val.c_str(), "signed") ||
    STRINGS_EQUAL(val.c_str(), "static") ||
    STRINGS_EQUAL(val.c_str(), "const") ||
    STRINGS_EQUAL(val.c_str(), "extern") ||
    STRINGS_EQUAL(val.c_str(), "__ignore_typecheck__");
}

int datatype::Padding(int val, int to)
{
  if (to <= 0)
  {
    return 0;
  }
  if ((val % to) == 0)
  {
    return 0;
  }
  return to - (val % to) % to;
}


