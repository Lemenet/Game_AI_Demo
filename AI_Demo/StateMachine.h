#pragma once

#include "State.h"


template<typename T>
class StateMachine
{
public:
	StateMachine(T* owner):
		m_pCurrentState(nullptr),
		m_pPreviousState(nullptr),
		m_pGlobalState(nullptr)
	{
		pOwner = owner;
	}

	virtual ~StateMachine() {}

	inline void SetCurrentState(State<T>* s) { m_pCurrentState = s; }

	inline void SetPreviousState(State<T>* s) { m_pPreviousState = s; }

	inline void SetGlobalState(State<T>* s) { m_pGlobalState = s; }

	inline State<T>* GetCurrentState() const { return m_pCurrentState; }

	inline State<T>* GetPreviousState() const { return m_pPreviousState; }

	inline State<T>* GetGlobalState() const { return m_pGlobalState; }

	void Update() const 
	{
		if (m_pGlobalState)
		{
			m_pGlobalState->Execute(pOwner);
		}

		if (m_pCurrentState)
		{
			m_pCurrentState->Execute(pOwner);
		}
	}

	bool HandleMessage(const Telegram & msg)
	{
		if (m_pCurrentState && m_pCurrentState->OnMessage(pOwner, msg))
		{
			return true;
		}

		if (m_pGlobalState && m_pGlobalState->OnMessage(pOwner, msg))
		{
			return true;
		}

		return false;
	}

	void ChangeState(State<T>* pNewState)
	{
		m_pPreviousState = m_pCurrentState;
		m_pCurrentState->Exit(pOwner);
		m_pCurrentState = pNewState;
		m_pCurrentState->Enter(pOwner);
	}

	void RevertToPreviousState()
	{
		ChangeState(m_pPreviousState);
	}

	void isInState(const State<T>* st) const
	{
		return typeid(*m_pCurrentState) == typeid(st);
	}

private:
	State<T>* m_pCurrentState;
	State<T>* m_pPreviousState;
	State<T>* m_pGlobalState;

	T* pOwner;
};







