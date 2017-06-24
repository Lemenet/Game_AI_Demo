#pragma once
#include "State.h"
#include "MinerWife.h"



class WifeGlobalState :
	public State<MinerWife>
{
public:
	WifeGlobalState() {}
	WifeGlobalState(const WifeGlobalState&);
	WifeGlobalState& operator = (const WifeGlobalState&);

public:
	static WifeGlobalState* Instance();

	virtual void Enter(MinerWife* wife);

	virtual void Execute(MinerWife* wife);

	virtual void Exit(MinerWife* wife);
};



//////////////////////////////////////////////////////////////////////////

class DoHouseWork :
	public State<MinerWife>
{
public:
	DoHouseWork() {}
	DoHouseWork(const DoHouseWork&);
	DoHouseWork& operator = (const DoHouseWork&);

public:
	static DoHouseWork* Instance();

	virtual void Enter(MinerWife* wife);

	virtual void Execute(MinerWife* wife);

	virtual void Exit(MinerWife* wife);
};


//////////////////////////////////////////////////////////////////////////


class VisitBathroom :
	public State<MinerWife>
{
public:
	VisitBathroom() {}
	VisitBathroom(const VisitBathroom&);
	VisitBathroom& operator = (const VisitBathroom&);

public:
	static VisitBathroom* Instance();

	virtual void Enter(MinerWife* wife);

	virtual void Execute(MinerWife* wife);

	virtual void Exit(MinerWife* wife);
};
