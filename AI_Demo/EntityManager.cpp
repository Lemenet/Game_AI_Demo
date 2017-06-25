#include "EntityManager.h"



EntityManager::EntityManager()
{
}


EntityManager::~EntityManager()
{
}

EntityManager * EntityManager::Instance()
{
	static EntityManager instance_;
	return &instance_;
}

void EntityManager::RegisterEntity(BaseGameEntity * NewEntity)
{
	m_EntityMap.insert(std::make_pair(NewEntity->GetID(), NewEntity));
}

BaseGameEntity * EntityManager::GetEntityFromId(int id) const
{
	EntityMap::const_iterator ent = m_EntityMap.find(id);
	return ent->second;
}

void EntityManager::RemoveEntity(BaseGameEntity * pEntity)
{
	m_EntityMap.erase(m_EntityMap.find(pEntity->GetID()));
}
