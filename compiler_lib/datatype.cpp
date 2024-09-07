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
  m_pointer_depth(0)
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
  m_file_position(file_position)
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
    return;
  }
  m_second = primitive_type;
  m_has_secondary_primitive_type = true;

  //FIXME: perform validation?

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


