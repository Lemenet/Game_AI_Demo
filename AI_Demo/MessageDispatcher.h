#pragma once

#include <set>
#include "BaseGameEntity.h"

class MessageDispatcher
{
private:
	std::set<Telegram> PriorityQ;		//消息队列，装消息的容器

	MessageDispatcher() {}

	MessageDispatcher(const MessageDispatcher&);
	MessageDispatcher& operator	= (const MessageDispatcher&);

public:
	static MessageDispatcher* Instance();

	void Discharge(BaseGameEntity* pReceiver, const Telegram& msg);
};