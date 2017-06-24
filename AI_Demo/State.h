#pragma once


template<typename T>
class State
{
public:
	virtual ~State() {}

	virtual void Enter(T*) = 0;
	virtual void Execute(T*) = 0;
	virtual void Exit(T*) = 0;
};

