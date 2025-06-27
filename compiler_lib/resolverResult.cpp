#include "pch.h"
#include "resolverResult.h"

resolverResult::resolverResult()
{
}

void resolverResult::addEntity(std::shared_ptr<resolverEntity> entity)
{
  if (!m_root_entity)
  {
    m_root_entity = entity;

  }
  m_last_entity = entity;
  m_entity = entity;
}

std::shared_ptr<resolverEntity> resolverResult::peekEntity()
{
  return m_entity;
}

std::shared_ptr<resolverEntity> resolverResult::nextEntity()
{
  return m_entity;
}
