#pragma once
#include "BaseGameEntity.h"
#include "Location.h"
#include "StateMachine.h"

class MinerWife : public BaseGameEntity
{
public:
	MinerWife(int id);
	virtual ~MinerWife();

	inline LocationType GetLocation() const { return m_location; }

	inline void SetLocation(LocationType loc) { m_location = loc; }

	inline StateMachine<MinerWife>* GetFSM() const { return m_pStateMachine; }

	virtual void Update();

	virtual bool HandleMessage(const Telegram& msg);

protected:
private:

	LocationType m_location;

	StateMachine<MinerWife>* m_pStateMachine;

// 	int m_iGoldCarried;
// 	int m_iMoneyBank;
// 	int m_iThirst;
// 	int m_iFatigue;

};