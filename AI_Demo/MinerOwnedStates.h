#pragma once

#include "State.h"
#include "Miner.h"

class GoHomeAndSleep : public State<Miner>
{
public:
	GoHomeAndSleep() {}
	GoHomeAndSleep(const GoHomeAndSleep&);
	GoHomeAndSleep& operator = (const GoHomeAndSleep&);

public:
	static GoHomeAndSleep* Instance();

	virtual void Enter(Miner* miner);

	virtual void Execute(Miner* miner);

	virtual void Exit(Miner* miner);
};


//-------------------------------------

//������ڿ�
class EnterMineAndDigForNugget : public State<Miner>
{
public:
	EnterMineAndDigForNugget() {}
	EnterMineAndDigForNugget(const EnterMineAndDigForNugget&);
	EnterMineAndDigForNugget& operator = (const EnterMineAndDigForNugget&);

public:
	static EnterMineAndDigForNugget* Instance();

	virtual void Enter(Miner* miner);

	virtual void Execute(Miner* miner);

	virtual void Exit(Miner* miner);
};


//----------------------

//ȥ���д�Ǯ
class VisitBankAndDepositGold : public State<Miner>
{
public:
	VisitBankAndDepositGold() {}
	VisitBankAndDepositGold(const VisitBankAndDepositGold&);
	VisitBankAndDepositGold& operator = (const VisitBankAndDepositGold&);

public:
	static VisitBankAndDepositGold* Instance();

	virtual void Enter(Miner* miner);

	virtual void Execute(Miner* miner);

	virtual void Exit(Miner* miner);
};


//-----------------------

//ȥ�ưɺȾ�
class QuenchThirst : public State<Miner>
{
public:
	QuenchThirst() {}
	QuenchThirst(const QuenchThirst&);
	QuenchThirst& operator = (const QuenchThirst&);

public:
	static QuenchThirst* Instance();

	virtual void Enter(Miner* miner);

	virtual void Execute(Miner* miner);

	virtual void Exit(Miner* miner);
};
