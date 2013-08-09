#include "systemclass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	SystemClass* System;
	bool result;

	System = new SystemClass;

	if(!System)
	{
		return 1;
	}

	if(System->Initialize())
	{
		System->Run();
	}

	System->Shutdown();
	delete System;
	System = 0;

	return 0;
}