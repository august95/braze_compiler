#include "../charStreamFile.h"
#include "../braze_compiler.h"


charStreamFile::~charStreamFile()
{
  m_file.close();
}
void charStreamFile::initialize(std::string filename)
{
  m_filename = filename;
  m_file_position.setFileName(m_filename);
  m_file.open(m_filename);
  if (!m_file.is_open())
  {
    std::cerr << "Failed to open file: " << m_filename << std::endl;
    // Handle error here
  }
}

char charStreamFile::peekChar()
{
  if (m_file.is_open())
  {
    return m_file.peek();
  }
  cerror("could not open test.c"); // add ...
  return 'x';
}

char charStreamFile::nextChar()
{
  if (m_file.is_open())
  {
    char c = m_file.get();
    m_file_position.incrementCol();
    if (c == '\n')
    {
      m_file_position.incrementLine();
    }
    return c;
  }

  cerror("could not open test.c"); // add ...
  return 'x';
}

void charStreamFile::pushChar(char ch)
{
  if (!m_file.is_open())
  {
    cerror("LEX error: could not open test.c"); // add ...
    return;
  }
  m_file.putback(ch);
}

bool charStreamFile::initialized()
{
  return m_file.is_open();
}

void charStreamFile::close()
{
  m_file.close();
}