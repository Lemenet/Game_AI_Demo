#include "MessageDispatcher.h"
#include <iostream>

MessageDispatcher * MessageDispatcher::Instance()
{
	static MessageDispatcher instance_;
	return &instance_;
}

void MessageDispatcher::Discharge(BaseGameEntity * pReceiver, const Telegram& msg)
{
	if (!pReceiver->HandleMessage(msg))
	{
		std::cout << "消息没被接管" << std::endl;
	}
}
