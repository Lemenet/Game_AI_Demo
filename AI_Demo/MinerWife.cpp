#include "MinerWife.h"
#include "MinerWifeOwnedState.h"
#include "ControlUtils.h"
#include <iostream>
#include <time.h>

using namespace std;

inline int RandInt(int min, int max)
{
	srand(time(0));
	return rand() % (max - min + 1) + min;
}


//////////////////////////////////////////////////////////////////////////



MinerWife::MinerWife(int id):
	BaseGameEntity(id),
	m_location(shack)
{
	m_pStateMachine = new StateMachine<MinerWife>(this);
	m_pStateMachine->SetCurrentState(DoHouseWork::Instance());
	m_pStateMachine->SetGlobalState(WifeGlobalState::Instance());
}

MinerWife::~MinerWife()
{
	delete m_pStateMachine;
}

void MinerWife::Update()
{
	SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);

// 	if (RandInt(0, 10) < 2)
// 	{
// 		GetFSM()->SetGlobalState(VisitBathroom::Instance());
// 	}
	GetFSM()->Update();
}

bool MinerWife::HandleMessage(const Telegram & msg)
{
	return false;
}
