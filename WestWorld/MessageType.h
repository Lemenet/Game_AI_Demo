#pragma once
#include <string>

enum Message_Type
{
	msg_ImHome,				//�󹤻�����
	msg_MealReady,			//��������
};


inline std::string MsgToStr(Message_Type msg)
{
	switch (msg)
	{
	case msg_ImHome:
		return "�����һ�����";

	case msg_MealReady:
		return "���Ѿ�����";

	default:
		return "Error";
	}
}