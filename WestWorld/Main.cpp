#include <iostream>
#include <windows.h>
#include "Miner.h"
#include "MinerWife.h"
#include "EntityName.h"
#include "EntityManager.h"
#include "MessageDispatcher.h"

using namespace std;

int main(int argc, char** argv)
{
	int i = 0;
	Miner* Bob = new Miner(name_Miner);
	MinerWife* Elsa = new MinerWife(name_Elsa);

	EntityM->RegisterEntity(Bob);
	EntityM->RegisterEntity(Elsa);

	while (i < 99)
	{
		system("cls");
		cout << i << endl;
		Bob->PrintInfor();
		Bob->Update();
		Elsa->Update();

		Dispatch->DispatchDelayMessage();
		++i;
		Sleep(1000);
	}

	getchar();
	return 0;
}