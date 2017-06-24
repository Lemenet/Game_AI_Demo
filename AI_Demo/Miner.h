#pragma once
#include "BaseGameEntity.h"
#include "Location.h"
#include "StateMachine.h"


const int confortLevel = 5;			//���ʵȼ�
const int MaxNuggets = 3;			//�ھ�Я����
const int ThirstyLevel = 5;			//�ڿʵȼ�
const int TirednessThreshold = 5;	//ƣ��

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

	//��ȡλ��
	inline LocationType GetLocation() const { return m_location; }

	//����λ��
	inline void SetLocation(const LocationType loc) { m_location = loc; }

	//��ȡЯ���Ľ��
	inline int GetGoldCarried() const { return m_iGoldCarried; }

	//����Я���Ľ��
	inline void SetGoldCarried(const int val) { m_iGoldCarried = val; }

	//����Я���Ľ������
	void AddToGoldCarried(const int val);

	//�жϿڴ��Ƿ�װ��
	inline bool PocketFull() const { return m_iGoldCarried >= MaxNuggets; }

	//�Ƿ�ﵽƣ������
	inline bool IsFatigue() const { return m_iFatigue >= TirednessThreshold; }

	//����ƣ��
	void DecreaseFatigue();

	//����ƣ��
	inline void IncreaseFatigue() { ++m_iFatigue; }

	//��ȡ���
	inline int GetWealth() const { return m_iMoneyBank; }

	//���ô��
	inline void SetWealth(const int val) { m_iMoneyBank = val; }

	//���Ӵ��
	void AddToWealth(const int val);

	//���ӿڿ�
	void IncreaseThirsty(const int val);

	//�Ƿ�ڿ�
	inline bool IsThirsty() const { return m_iThirst >= ThirstyLevel; }

	//��һƿ��ʿ��
	void BuyAndDrinkWhiskey()
	{
		m_iThirst = 0;
		m_iMoneyBank -= 2;
	}
};

