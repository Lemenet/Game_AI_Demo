#include <iostream>
#include <time.h>
#include "MinerWifeOwnedState.h"
#include "EntityName.h"
#include "MessageType.h"
#include "MessageDispatcher.h"

using namespace std;

inline int RandInt(int min, int max)
{
	srand(static_cast<unsigned int>(time(0)));
	return rand() % (max - min + 1) + min;
}


//////////////////////////////////////////////////////////////////////////


WifeGlobalState * WifeGlobalState::Instance()
{
	static WifeGlobalState instance_;
	return &instance_;
}

void WifeGlobalState::Enter(MinerWife * wife)
{
	if (wife->GetLocation() != bathroom)
	{
		wife->SetLocation(bathroom);
		cout << GetEntityName(name_Elsa) << "�������" << endl;
	}
}

void WifeGlobalState::Execute(MinerWife * wife)
{
	if (RandInt(0, 10) < 2)
	{
		wife->GetFSM()->ChangeState(VisitBathroom::Instance());
	}

}

void WifeGlobalState::Exit(MinerWife * wife)
{
	cout << "�뿪����" << endl;
}

bool WifeGlobalState::OnMessage(MinerWife *, const Telegram &)
{
	cout << "\n Elsa 1" << endl;
	return true;
}




//////////////////////////////////////////////////////////////////////////



DoHouseWork * DoHouseWork::Instance()
{
	static DoHouseWork instance_;
	return &instance_;
}

void DoHouseWork::Enter(MinerWife * wife)
{
	if (wife->GetLocation() != shack)
	{
		wife->SetLocation(shack);
		cout << GetEntityName(name_Elsa) << "��ʼ������" << endl;
	}
}

void DoHouseWork::Execute(MinerWife * wife)
{
	switch (RandInt(0, 2))
	{
	case 0:
		cout << "����" << endl;
		break;

	case 1:
		cout << "ϴ��" << endl;
		break;

	case 2:
		cout << "�ϵ�" << endl;
		break;

	default:
		cout << "Random Error!!" << endl;
		break;
	}
}

void DoHouseWork::Exit(MinerWife * wife)
{
	cout << "ֹͣ������" << endl;
}

bool DoHouseWork::OnMessage(MinerWife * minerWife, const Telegram & receivedTelegram)
{
	switch (receivedTelegram.msg)
	{
	case msg_ImHome:
		Dispatch->DispatchMessage(3, minerWife->GetID(), name_Elsa, msg_MealReady, "����������������Ϣ");
		return true;

	case msg_MealReady:
		Dispatch->DispatchMessage(0, minerWife->GetID(), name_Miner, msg_MealReady, "�������ˣ����Է���");
		return true;

	default:
		break;
	}
	return false;
}



//////////////////////////////////////////////////////////////////////////



VisitBathroom * VisitBathroom::Instance()
{
	static VisitBathroom instance_;
	return &instance_;
}

void VisitBathroom::Enter(MinerWife * wife)
{
	if (wife->GetLocation() != bathroom)
	{
		wife->SetLocation(bathroom);
		cout << GetEntityName(name_Elsa) << "�������" << endl;
	}
}

void VisitBathroom::Execute(MinerWife * wife)
{
	cout << "�����ϲ���" << endl;
	wife->GetFSM()->RevertToPreviousState();

}

void VisitBathroom::Exit(MinerWife * wife)
{
	cout << "�뿪����" << endl;
}

bool VisitBathroom::OnMessage(MinerWife * minerWife, const Telegram & receivedTelegram)
{
	switch (receivedTelegram.msg)
	{
	case msg_ImHome:
		cout << "����������������Ϣ" << endl;
		Dispatch->DispatchMessage(3, name_Elsa, name_Elsa, msg_MealReady, nullptr);
		return true;

	case msg_MealReady:
		cout << "�������ˣ����Է���" << endl;
		Dispatch->DispatchMessage(0, minerWife->GetID(), name_Miner, msg_MealReady, nullptr);
		return true;

	default:
		break;
	}

	return false;
}
