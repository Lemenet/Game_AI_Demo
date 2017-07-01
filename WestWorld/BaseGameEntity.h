#pragma once

#include "Telegram.h"

class BaseGameEntity
{
private:
	int m_ID;
	static int iNextValidID;

	void SetID(int id);

public:
	BaseGameEntity(int id);
	virtual ~BaseGameEntity();

	int GetID() const;

public:
	virtual void Update() = 0;

	virtual bool HandleMessage(const Telegram& msg) = 0;

};

