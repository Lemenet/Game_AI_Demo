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

	//矿工的状态机
	StateMachine<Miner>* m_pStateMachine;

	//矿工携带的金币数量
	int m_iGoldCarried;

	//矿工银行存款的数量
	int m_iMoneyBank;

	//当天挖到的金矿数量
	int m_goldToday;

	//矿工口渴度
	int m_iThirst;

	//矿工疲劳值
	int m_iFatigue;

	//回家是否吃饭了
	bool m_bHasMeal;


public:

	//更新主循环-----主要事件驱动函数，需要循环执行
	virtual void Update();

	//处理消息
	virtual bool HandleMessage(const Telegram& msg);

	/*void ChangeState(State<Miner>* state);*/

	//向控制台打印矿工的所有信息
	void PrintInfor();

	//获取矿工的状态机实例
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

	//疲劳是否清除（疲劳为0）
	inline bool IsEnergetic() const { return m_iFatigue <= 0; }

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

	//获取今天采矿的数量
	inline int GetGoldToday() const { return m_goldToday; }

	//增加今天挖到的金矿数
	inline void AddGoldToday(const int val = 1) { m_goldToday += val; }

	//将今天挖到的金矿数清零
	inline void InitialGoldToday() { m_goldToday = 0; }

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

	//查看是否吃饭了
	inline bool HasMealed() const { return m_bHasMeal; }

	//设置是否吃饭
	inline void SetMealState(bool state) { m_bHasMeal = state; }
};

