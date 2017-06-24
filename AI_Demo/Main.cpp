#include <iostream>
#include <windows.h>
#include "Miner.h"
#include "MinerWife.h"
//#include "EntityName.h"

using namespace std;

int main(int argc, char** argv)
{
	int i = 0;
	Miner miner(100);
	MinerWife wife(101);
	while (i < 20)
	{
		//system("cls");
		//cout << i << endl;
		//miner.PrintInfor();
		miner.Update();
		wife.Update();
		++i;
		Sleep(1000);
	}

	getchar();
	return 0;
}