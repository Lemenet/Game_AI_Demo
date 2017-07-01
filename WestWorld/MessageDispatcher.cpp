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
		std::cout << "Debug: Message to "<< GetEntityName(pReceiver->GetID()) <<" ��Ϣû���ӹ�,�ѱ�����" << std::endl;
	}
}

void MessageDispatcher::DispatchMessage(double delay, int sender, int receiver, int msg, void * extraInfo)
{
	SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	BaseGameEntity* pSender = EntityM->GetEntityFromId(sender);
	BaseGameEntity* pReceiver = EntityM->GetEntityFromId(receiver);
	if (nullptr == pReceiver)
	{
		std::cout << "���棡�����߲�����" << receiver << std::endl;
	}
	Telegram telegram(0, sender, receiver, msg, extraInfo);

	if (delay <= 0.f)
	{
		//std::cout << "\n" << GetEntityName(pSender->GetID()) << "------->>>" << GetEntityName(pReceiver->GetID()) << "��Ϣ������" << MsgToStr((Message_Type)msg) << endl;
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
		//std::cout << "\n��ʱ����" << GetEntityName(pSender->GetID()) << "��ʱ��Ӧʱ��Ϊ" << Clock->GetCurrentTime() << "for" << GetEntityName(pReceiver->GetID()) << "Msg is" << MsgToStr((Message_Type)msg);
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

		//std::cout << "\n������Ϣ�Ѿ������ͣ����͵���" << GetEntityName(pReceiver->GetID()) << "Msg is " << MsgToStr((Message_Type)telegram.msg);
		Discharge(pReceiver, telegram);
		PriorityQ.erase(PriorityQ.begin());
	}
}
