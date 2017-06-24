#include "BaseGameEntity.h"

int BaseGameEntity::iNextValidID = 0;

void BaseGameEntity::SetID(int id)
{
	iNextValidID = m_ID + 1;
	m_ID = id;
}

BaseGameEntity::BaseGameEntity(int id)
{
	SetID(id);
}


BaseGameEntity::~BaseGameEntity()
{
}

int BaseGameEntity::GetID() const
{
	return m_ID;
}
