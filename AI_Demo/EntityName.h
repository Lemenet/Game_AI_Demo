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
		return string("¿ó¹¤");
	case name_Elsa:
		return string("°¬ÀöÈø");
	default:
		return string("Error");
	}
}
