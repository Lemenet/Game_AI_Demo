#pragma once
#include <math.h>
#include <iostream>

struct Telegram
{
	int	sender;				//发送者
	int receiver;			//接收者

	int msg;				//消息编号
	double dispatchTime;
	void* extraInfo;

	Telegram() :
		sender(-1),
		receiver(-1),
		msg(-1),
		dispatchTime(-1)
	{}

	Telegram(double _dispatchTime, int _sender, int _receiver, int _msg, void* _extraInfo = nullptr) :
		sender(_sender),
		receiver(_receiver),
		msg(_msg),
		dispatchTime(_dispatchTime),
		extraInfo(_extraInfo)
	{}
};

const double SmallestDelay = 0.25;

inline bool operator == (const Telegram& t1, const Telegram& t2)
{
	return (fabs(t1.dispatchTime - t2.dispatchTime) < SmallestDelay) &&
		(t1.sender == t2.sender) &&
		(t1.receiver == t2.receiver) &&
		(t1.msg == t2.msg);
}

inline bool operator< (const Telegram& t1, const Telegram& t2)
{
	if (t1 == t2)
	{
		return false;
	}
	else
	{
		return (t1.dispatchTime < t2.dispatchTime);
	}
}

inline std::ostream& operator<< (std::ostream& os, const Telegram& t)
{
	os << "time:" << t.dispatchTime << "Sender:" << t.sender << "Receiver:" << t.receiver << "Msg:" << t.msg;
	return os;
}

template<class T>
inline T DereferenceToType(void* p)
{
	return *(T*)(p);
}