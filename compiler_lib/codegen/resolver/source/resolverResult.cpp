#include "../../source/pch.h"
#include "../resolverResult.h"

void resolverResult::addEntity(std::shared_ptr<resolverEntity> entity)
{
  if (m_entities.empty())
  {
    m_root_entity_data = entity->getResolverEntityData();
  }
  m_entities.push_back(entity);
}

std::shared_ptr<resolverEntity> resolverResult::peekEntity()
{
  if (m_entities.empty())
  {
    return std::shared_ptr<resolverEntity>();
  }
  return m_entities.back();
}

std::shared_ptr<resolverEntity> resolverResult::nextEntity()
{
  if (m_entities.empty())
  {
    return std::shared_ptr<resolverEntity>();
  }
  std::shared_ptr<resolverEntity> entity = m_entities.back();
  m_entities.pop_back();
  return entity;
}

std::shared_ptr<resolverEntity> resolverResult::getRootEntity()
{
  if (m_entities.empty())
  {
    return std::shared_ptr<resolverEntity>();
  }
  return m_entities.front();
}

std::string resolverResult::getRootAddress()
{
  return m_root_entity_data->getAddress();
}
