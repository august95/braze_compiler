#include "../nodeStructDeclaration.h"

nodeStructDeclaration::nodeStructDeclaration(std::string name, filePosition file_position)
  : node(NODE_TYPE_STRUCT, file_position),
    m_size(0),
    m_alignment(1)
{
  setStringValue(name);
}

void nodeStructDeclaration::addMember(std::shared_ptr<nodeVariableDeclaration> member)
{
  int member_size = member->getDatatypeSize();
  int alignment = member->getDatatype()->getDatatypeAlignment();
  m_size += datatype::Padding(m_size, alignment);
  member->setStructMemberOffset(m_size);
  m_members.push_back(member);
  m_size += member_size;
  if (alignment > m_alignment)
  {
    m_alignment = alignment;
  }
}

std::shared_ptr<nodeVariableDeclaration> nodeStructDeclaration::findMember(const std::string& name)
{
  for (const auto& member : m_members)
  {
    if (member->getStringValue() == name)
    {
      return member;
    }
  }
  return std::shared_ptr<nodeVariableDeclaration>();
}
