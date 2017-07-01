#pragma once

#include <set>
#include "BaseGameEntity.h"


#define Dispatch MessageDispatcher::Instance()

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

	void DispatchMessage(double delay,	//消息延迟
		int sender,						//发送者
		int receiver,					//接收者
		int msg,						//消息编号
		void* extraInfo					//额外信息
	);

	void DispatchDelayMessage();
};