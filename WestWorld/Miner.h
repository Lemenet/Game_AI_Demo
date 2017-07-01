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

	//�󹤵�״̬��
	StateMachine<Miner>* m_pStateMachine;

	//��Я���Ľ������
	int m_iGoldCarried;

	//�����д�������
	int m_iMoneyBank;

	//�����ڵ��Ľ������
	int m_goldToday;

	//�󹤿ڿʶ�
	int m_iThirst;

	//��ƣ��ֵ
	int m_iFatigue;

	//�ؼ��Ƿ�Է���
	bool m_bHasMeal;


public:

	//������ѭ��-----��Ҫ�¼�������������Ҫѭ��ִ��
	virtual void Update();

	//������Ϣ
	virtual bool HandleMessage(const Telegram& msg);

	/*void ChangeState(State<Miner>* state);*/

	//�����̨��ӡ�󹤵�������Ϣ
	void PrintInfor();

	//��ȡ�󹤵�״̬��ʵ��
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

	//ƣ���Ƿ������ƣ��Ϊ0��
	inline bool IsEnergetic() const { return m_iFatigue <= 0; }

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

	//��ȡ����ɿ������
	inline int GetGoldToday() const { return m_goldToday; }

	//���ӽ����ڵ��Ľ����
	inline void AddGoldToday(const int val = 1) { m_goldToday += val; }

	//�������ڵ��Ľ��������
	inline void InitialGoldToday() { m_goldToday = 0; }

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

	//�鿴�Ƿ�Է���
	inline bool HasMealed() const { return m_bHasMeal; }

	//�����Ƿ�Է�
	inline void SetMealState(bool state) { m_bHasMeal = state; }
};

