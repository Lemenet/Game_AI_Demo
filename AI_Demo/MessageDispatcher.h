#pragma once

#include <set>
#include "BaseGameEntity.h"


#define Dispatch MessageDispatcher::Instance()

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

	void DispatchMessage(double delay,	//��Ϣ�ӳ�
		int sender,						//������
		int receiver,					//������
		int msg,						//��Ϣ���
		void* extraInfo					//������Ϣ
	);

	void DispatchDelayMessage();
};