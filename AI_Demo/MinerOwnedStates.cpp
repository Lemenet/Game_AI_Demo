#include "MinerOwnedStates.h"
#include <iostream>
using namespace std;


GoHomeAndSleep * GoHomeAndSleep::Instance()
{
	static GoHomeAndSleep instance_;
	return &instance_;
}

void GoHomeAndSleep::Enter(Miner * miner)
{
	if (miner->GetLocation() != goldMine)
	{
		miner->SetLocation(goldMine);
		cout << "�ؼ�׼��˯��" << endl;
	}
}

void GoHomeAndSleep::Execute(Miner * miner)
{
	miner->DecreaseFatigue();
	cout << "�ڼ�˯��" << endl;
	//miner->ChangeState(VisitBankAndDepositGold::Instance());
}

void GoHomeAndSleep::Exit(Miner * miner)
{
	cout << "�뿪��" << endl;
}


//---------------------------


EnterMineAndDigForNugget * EnterMineAndDigForNugget::Instance()
{
	static EnterMineAndDigForNugget instance_;
	return &instance_;
}

void EnterMineAndDigForNugget::Enter(Miner * miner)
{
	if (miner->GetLocation() != goldMine)
	{
		miner->SetLocation(goldMine);
		cout << "�����׼���ڿ�" << endl;
	}
}

void EnterMineAndDigForNugget::Execute(Miner * miner)
{
	miner->AddToGoldCarried(1);			//�ڿ�����1���
	cout << "���ڿ��ڿ�" << endl;
	miner->IncreaseFatigue();			//ƣ������1
	if (miner->PocketFull())
	{
		cout << "�ڴ����ˣ�Ҫȥ��Ǯ" << endl;
		miner->GetFSM()->ChangeState(VisitBankAndDepositGold::Instance());
	}
	if (miner->IsThirsty())
	{
		cout <<  "�ڿ��ˣ�Ҫȥ�ưɺȾ�" << endl;
		miner->GetFSM()->ChangeState(QuenchThirst::Instance());
	}
}

void EnterMineAndDigForNugget::Exit(Miner * miner)
{
	cout << "�뿪��" << endl;
}


//------------------------------


VisitBankAndDepositGold * VisitBankAndDepositGold::Instance()
{
	static VisitBankAndDepositGold instance_;
	return &instance_;
}

void VisitBankAndDepositGold::Enter(Miner * miner)
{
	if (miner->GetLocation() != bank)
	{
		miner->SetLocation(bank);
		cout << "��������׼����Ǯ" << endl;
	}
}

void VisitBankAndDepositGold::Execute(Miner * miner)
{
	cout << "�����д�Ǯ���" << endl;

	miner->AddToWealth(miner->GetGoldCarried());
	miner->SetGoldCarried(0);

	/*cout << "������" << miner->GetWealth() << endl;*/
	if (miner->GetWealth() >= 5)
	{
		cout << "�����⣬���칤������" << endl;
		miner->GetFSM()->ChangeState(GoHomeAndSleep::Instance());
	}
	else
	{
		miner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
	}
}

void VisitBankAndDepositGold::Exit(Miner * miner)
{
	cout << "�뿪����" << endl;
}


//-----------------------

QuenchThirst * QuenchThirst::Instance()
{
	static QuenchThirst instance_;
	return &instance_;
}

void QuenchThirst::Enter(Miner * miner)
{
	if (miner->GetLocation() != saloon)
	{
		miner->SetLocation(saloon);
		cout << "����ưɺȾ�" << endl;
	}
}

void QuenchThirst::Execute(Miner * miner)
{
	miner->BuyAndDrinkWhiskey();		//����ʿ�ɺȣ��ڿ����㣬Wealth����2
	cout << "�ھưɺȾƣ��о�������" << endl;
	miner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
}

void QuenchThirst::Exit(Miner * miner)
{
	cout << "�뿪�ư�" << endl;
}
