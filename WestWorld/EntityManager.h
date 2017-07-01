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
	//单例模式
	static EntityManager* Instance();

	//注册一个角色实体
	void RegisterEntity(BaseGameEntity* NewEntity);

	//根据ID获取角色实体
	BaseGameEntity* GetEntityFromId(int id) const;

	//将角色实体移除
	void RemoveEntity(BaseGameEntity* pEntity);
};

