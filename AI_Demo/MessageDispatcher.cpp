#include "MessageDispatcher.h"
#include "EntityManager.h"
#include "EntityName.h"
#include "MessageType.h"
#include "ControlUtils.h"
#include "CrudeTimer.h"
#include <iostream>

#undef DispatchMessage


MessageDispatcher * MessageDispatcher::Instance()
{
	static MessageDispatcher instance_;
	return &instance_;
}

void MessageDispatcher::Discharge(BaseGameEntity * pReceiver, const Telegram& msg)
{
	if (!pReceiver->HandleMessage(msg))
	{
		std::cout << "Debug: Message to "<< GetEntityName(pReceiver->GetID()) <<" 消息没被接管,已被丢弃" << std::endl;
	}
}

void MessageDispatcher::DispatchMessage(double delay, int sender, int receiver, int msg, void * extraInfo)
{
	SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	BaseGameEntity* pSender = EntityM->GetEntityFromId(sender);
	BaseGameEntity* pReceiver = EntityM->GetEntityFromId(receiver);
	if (nullptr == pReceiver)
	{
		std::cout << "警告！接收者不存在" << receiver << std::endl;
	}
	Telegram telegram(0, sender, receiver, msg, extraInfo);

	if (delay <= 0.f)
	{
		//std::cout << "\n" << GetEntityName(pSender->GetID()) << "------->>>" << GetEntityName(pReceiver->GetID()) << "消息内容是" << MsgToStr((Message_Type)msg) << endl;
		if (nullptr != extraInfo)
		{
			std::cout << std::string((char*)extraInfo) << endl;
		}
		Discharge(pReceiver, telegram);
	}
	else
	{
		double currentTime = Clock->GetCurrentTime();

		telegram.dispatchTime = delay + currentTime;
		PriorityQ.insert(telegram);
		//std::cout << "\n延时处理" << GetEntityName(pSender->GetID()) << "延时响应时间为" << Clock->GetCurrentTime() << "for" << GetEntityName(pReceiver->GetID()) << "Msg is" << MsgToStr((Message_Type)msg);
	}

}

void MessageDispatcher::DispatchDelayMessage()
{
	SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	double CurrentTime = Clock->GetCurrentTime();
	while (!PriorityQ.empty() && 
		(PriorityQ.begin()->dispatchTime < CurrentTime) && 
		(PriorityQ.begin()->dispatchTime > 0))
	{
		const Telegram& telegram = *PriorityQ.begin();
		BaseGameEntity* pReceiver = EntityM->GetEntityFromId(telegram.receiver);

		//std::cout << "\n队列消息已经被发送，发送到：" << GetEntityName(pReceiver->GetID()) << "Msg is " << MsgToStr((Message_Type)telegram.msg);
		Discharge(pReceiver, telegram);
		PriorityQ.erase(PriorityQ.begin());
	}
}
