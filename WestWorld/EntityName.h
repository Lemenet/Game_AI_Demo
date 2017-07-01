#pragma once

#include <string>
using namespace std;


enum Name
{
	name_Miner = 100,
	name_Elsa,
};


inline string GetEntityName(int name)
{
	switch (name)
	{
	case name_Miner:
		return string("��");
	case name_Elsa:
		return string("������");
	default:
		return string("Error");
	}
}
