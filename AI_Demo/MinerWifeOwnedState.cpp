#include "MinerWifeOwnedState.h"
#include "EntityName.h"
#include <iostream>
#include <time.h>

using namespace std;

inline int RandInt(int min, int max)
{
	srand(time(0));
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
		cout << GetEntityName(Elsa) << "�������" << endl;
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
		cout << GetEntityName(Elsa) << "�����" << endl;
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
	cout << "�뿪��" << endl;
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
		cout << GetEntityName(Elsa) << "�������" << endl;
	}
}

void VisitBathroom::Execute(MinerWife * wife)
{
	cout << "ȥ�ϲ���" << endl;
	wife->GetFSM()->RevertToPreviousState();

}

void VisitBathroom::Exit(MinerWife * wife)
{
	cout << "�뿪����" << endl;
}
