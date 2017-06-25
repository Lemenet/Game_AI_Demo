#pragma once
#include "BaseGameEntity.h"

#include <map>

#define EntityM EntityManager::Instance()

class EntityManager
{
private:
	typedef std::map<int, BaseGameEntity*> EntityMap;

private:
	EntityMap m_EntityMap;



	EntityManager();
	~EntityManager();
	EntityManager(const EntityManager&);
	EntityManager& operator = (const EntityManager&);

public:
	//����ģʽ
	static EntityManager* Instance();

	//ע��һ����ɫʵ��
	void RegisterEntity(BaseGameEntity* NewEntity);

	//����ID��ȡ��ɫʵ��
	BaseGameEntity* GetEntityFromId(int id) const;

	//����ɫʵ���Ƴ�
	void RemoveEntity(BaseGameEntity* pEntity);
};

