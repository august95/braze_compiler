#include "pch.h"
#include "datatype.h"
#include <assert.h>
#include "braze_compiler.h"

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
  m_datatype_size(0)
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
  m_datatype_size(0)
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
    size = 0;
    break;

  case primitiveType::DATA_TYPE_VOID:
    size = 0;
    break;

  case primitiveType::DATA_TYPE_CHAR:
    size = 1;
    break;

  case primitiveType::DATA_TYPE_SHORT:
    size = 2;
    break;

  case primitiveType::DATA_TYPE_INTEGER:
    size = 4;
    break;

  case primitiveType::DATA_TYPE_LONG:
    size = 4;
    break;

  case primitiveType::DATA_TYPE_FLOAT:
    size = 4;
    break;

  case primitiveType::DATA_TYPE_DOUBLE:
    size = 4;
    break;

  case primitiveType::DATA_TYPE_UNKNOWN:
    size = 0;
    break;
  
  }
  return size;
}


bool datatype::IsKeywordDatatype(std::string val)
{
  return STRINGS_EQUAL(val.c_str(), "void") ||
    STRINGS_EQUAL(val.c_str(), "char") ||
    STRINGS_EQUAL(val.c_str(), "int") ||
    STRINGS_EQUAL(val.c_str(), "short") ||
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


