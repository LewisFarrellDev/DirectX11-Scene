#include "SystemClass.h"
#include <stdio.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment (lib, "directxtk.lib")

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLineArgs, int nInitialWinShowState) 
{
	//Add the following source code when you are using DirectXTK WIC in your program
	HRESULT hrInit = CoInitializeEx(NULL, COINIT_MULTITHREADED);

	if (hrInit != S_OK)
	{
		MessageBox(NULL, L"Unable to initialise for directXTK WIC (main.CPP)", L"Error", MB_OK | MB_ICONERROR);
		return 0;
	}
	
	SystemClass *systemClass = new SystemClass();
	if (systemClass)
	{
		if (systemClass->InitializeSystem(hInstance, nInitialWinShowState))
		{
			systemClass->MessageLoop();
		}
	}

	if (systemClass)
		systemClass->ShutdownSystem();

	return 0;
}