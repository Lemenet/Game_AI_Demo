#pragma once
#include <string>

enum Message_Type
{
	msg_ImHome,				//矿工回来了
	msg_MealReady,			//饭做好了
};


inline std::string MsgToStr(Message_Type msg)
{
	switch (msg)
	{
	case msg_ImHome:
		return "宝贝我回来了";

	case msg_MealReady:
		return "饭已经好了";

	default:
		return "Error";
	}
}