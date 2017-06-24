#pragma once
#include "BaseGameEntity.h"
#include "Location.h"
#include "StateMachine.h"


const int confortLevel = 5;			//舒适等级
const int MaxNuggets = 3;			//挖掘携带量
const int ThirstyLevel = 5;			//口渴等级
const int TirednessThreshold = 5;	//疲劳

class Miner : public BaseGameEntity
{
public:
	Miner(int id);
	~Miner();


private:
	//State<Miner>* m_pCurrentState;
	LocationType m_location;

	StateMachine<Miner>* m_pStateMachine;

	int m_iGoldCarried;
	int m_iMoneyBank;
	int m_iThirst;
	int m_iFatigue;


public:

	virtual void Update();

	virtual bool HandleMessage(const Telegram& msg);

	/*void ChangeState(State<Miner>* state);*/

	void PrintInfor();

	StateMachine<Miner>* GetFSM() const { return m_pStateMachine; }

	//获取位置
	inline LocationType GetLocation() const { return m_location; }

	//设置位置
	inline void SetLocation(const LocationType loc) { m_location = loc; }

	//获取携带的金币
	inline int GetGoldCarried() const { return m_iGoldCarried; }

	//设置携带的金币
	inline void SetGoldCarried(const int val) { m_iGoldCarried = val; }

	//增加携带的金币数量
	void AddToGoldCarried(const int val);

	//判断口袋是否装满
	inline bool PocketFull() const { return m_iGoldCarried >= MaxNuggets; }

	//是否达到疲劳上限
	inline bool IsFatigue() const { return m_iFatigue >= TirednessThreshold; }

	//减少疲劳
	void DecreaseFatigue();

	//增加疲劳
	inline void IncreaseFatigue() { ++m_iFatigue; }

	//获取存款
	inline int GetWealth() const { return m_iMoneyBank; }

	//设置存款
	inline void SetWealth(const int val) { m_iMoneyBank = val; }

	//增加存款
	void AddToWealth(const int val);

	//增加口渴
	void IncreaseThirsty(const int val);

	//是否口渴
	inline bool IsThirsty() const { return m_iThirst >= ThirstyLevel; }

	//买一瓶威士忌
	void BuyAndDrinkWhiskey()
	{
		m_iThirst = 0;
		m_iMoneyBank -= 2;
	}
};

