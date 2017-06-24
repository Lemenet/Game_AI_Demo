#pragma once

#include <set>
#include "BaseGameEntity.h"

class MessageDispatcher
{
private:
	std::set<Telegram> PriorityQ;		//��Ϣ���У�װ��Ϣ������

	MessageDispatcher() {}

	MessageDispatcher(const MessageDispatcher&);
	MessageDispatcher& operator	= (const MessageDispatcher&);

public:
	static MessageDispatcher* Instance();

	void Discharge(BaseGameEntity* pReceiver, const Telegram& msg);
};