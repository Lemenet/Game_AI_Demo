#include "Miner.h"
#include "MinerOwnedStates.h"
#include "ControlUtils.h"
#include <iostream>

using namespace std;


Miner::Miner(int id) :
	BaseGameEntity(id),
	m_location(shack),
	m_iGoldCarried(0),
	m_iMoneyBank(0),
	m_iThirst(0),
	m_iFatigue(0)
	{
		m_pStateMachine = new StateMachine<Miner>(this);
		m_pStateMachine->SetCurrentState(EnterMineAndDigForNugget::Instance());

	}


Miner::~Miner()
{

}

void Miner::Update()
{
	SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
	++m_iThirst;
	GetFSM()->Update();
}

bool Miner::HandleMessage(const Telegram & msg)
{
	return false;
}


void Miner::PrintInfor()
{
	cout << "疲劳： " << m_iFatigue << endl;
	cout << "口渴： " << m_iThirst << endl;
	cout << "存款：" << m_iMoneyBank << endl;
	cout << "口袋：" << m_iGoldCarried << endl;
}

void Miner::AddToGoldCarried(const int val)
{
	m_iGoldCarried += val;
	if (m_iGoldCarried < 0)
	{
		m_iGoldCarried = 0;
	}
}

void Miner::DecreaseFatigue()
{
	--m_iFatigue;
	if (m_iFatigue < 0)
	{
		m_iFatigue = 0;
	}
}

void Miner::AddToWealth(const int val)
{
	m_iMoneyBank += val;
	if (m_iMoneyBank < 0)
	{
		m_iMoneyBank = 0;
	}
}

void Miner::IncreaseThirsty(const int val)
{
	m_iThirst += val;
	if (m_iThirst < 0)
	{
		m_iThirst = 0;
	}
}
