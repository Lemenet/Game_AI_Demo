#pragma once
#include <string>
using namespace std;

enum Name
{
	Miner = 100,
	Elsa,
};

string GetEntityName(int name)
{
	switch (name)
	{
	case Miner:
		return string("��");
	case Elsa:
		return string("������");
	default:
		return string("Error");
	}
}
