#pragma once
#include <string>
#include "filePosition.h"
class charStreamInterface
{
public:
  virtual char peekChar() = 0;
  virtual char nextChar() = 0;
  virtual void pushChar(char ch) = 0;
  virtual bool initialized() = 0;
  virtual void close() {};
  filePosition getFilePostiion() { return m_file_position; }

protected:
  filePosition m_file_position;
};

