#pragma once
#include <string>
#include "filePosition.h"

enum primitiveType
{
  DATA_TYPE_NONE,
  DATA_TYPE_VOID,
  DATA_TYPE_CHAR,
  DATA_TYPE_SHORT,
  DATA_TYPE_INTEGER,
  DATA_TYPE_LONG,
  DATA_TYPE_FLOAT,
  DATA_TYPE_DOUBLE,
  DATA_TYPE_STRUCT,
  DATA_TYPE_UNION,
  DATA_TYPE_UNKNOWN
};


class datatype
{
public:
  datatype();
  datatype(filePosition file_position);


  void setKeyVariableModifier(std::string modifier);
  void setDataType(std::string data_type);

  bool isSigned() { return m_signed; }
  bool isUnsigned() { return m_unsigned; }
  bool isStatic() { return m_static; }
  bool isConst() { return m_const; }
  bool isExtern() { return m_extern; }
  bool secondaryPrimitiveType() { return m_has_secondary_primitive_type; }
  void incrementPointerDepth() { m_pointer_depth++; }
  bool isUnion() { return m_first == primitiveType::DATA_TYPE_UNION; }
  bool isStruct() { return m_first == primitiveType::DATA_TYPE_STRUCT; }


  static bool IsKeywordDatatype(std::string val);
  static bool isKeywordVariableModifier(std::string val);


private:

  primitiveType m_first;
  primitiveType m_second;
  bool m_has_secondary_primitive_type;

  bool m_unsigned;
  bool m_signed;
  bool m_static;
  bool m_const;
  bool m_extern;
  unsigned int m_pointer_depth;
  filePosition m_file_position;

};
