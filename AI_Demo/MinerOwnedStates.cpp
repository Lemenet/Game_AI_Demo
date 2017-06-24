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
		cout << "回家准备睡觉" << endl;
	}
}

void GoHomeAndSleep::Execute(Miner * miner)
{
	miner->DecreaseFatigue();
	cout << "在家睡觉" << endl;
	//miner->ChangeState(VisitBankAndDepositGold::Instance());
}

void GoHomeAndSleep::Exit(Miner * miner)
{
	cout << "离开家" << endl;
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
		cout << "进入矿洞准备挖矿" << endl;
	}
}

void EnterMineAndDigForNugget::Execute(Miner * miner)
{
	miner->AddToGoldCarried(1);			//挖矿增加1金币
	cout << "正在矿洞挖矿" << endl;
	miner->IncreaseFatigue();			//疲劳增加1
	if (miner->PocketFull())
	{
		cout << "口袋满了，要去存钱" << endl;
		miner->GetFSM()->ChangeState(VisitBankAndDepositGold::Instance());
	}
	if (miner->IsThirsty())
	{
		cout <<  "口渴了，要去酒吧喝酒" << endl;
		miner->GetFSM()->ChangeState(QuenchThirst::Instance());
	}
}

void EnterMineAndDigForNugget::Exit(Miner * miner)
{
	cout << "离开矿洞" << endl;
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
		cout << "进入银行准备存钱" << endl;
	}
}

void VisitBankAndDepositGold::Execute(Miner * miner)
{
	cout << "在银行存钱完毕" << endl;

	miner->AddToWealth(miner->GetGoldCarried());
	miner->SetGoldCarried(0);

	/*cout << "存款余额" << miner->GetWealth() << endl;*/
	if (miner->GetWealth() >= 5)
	{
		cout << "很满意，今天工作结束" << endl;
		miner->GetFSM()->ChangeState(GoHomeAndSleep::Instance());
	}
	else
	{
		miner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
	}
}

void VisitBankAndDepositGold::Exit(Miner * miner)
{
	cout << "离开银行" << endl;
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
		cout << "进入酒吧喝酒" << endl;
	}
}

void QuenchThirst::Execute(Miner * miner)
{
	miner->BuyAndDrinkWhiskey();		//买威士忌喝，口渴清零，Wealth减少2
	cout << "在酒吧喝酒，感觉不渴了" << endl;
	miner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
}

void QuenchThirst::Exit(Miner * miner)
{
	cout << "离开酒吧" << endl;
}
