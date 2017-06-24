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
		return string("¿ó¹¤");
	case Elsa:
		return string("°¬ÀöÈø");
	default:
		return string("Error");
	}
}
