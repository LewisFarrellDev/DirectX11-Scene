#include "SystemClass.h"
#include <stdio.h>

SystemClass::SystemClass()
{

}

bool SystemClass::InitializeSystem(HINSTANCE hInstance, int nInitialWinShowState)
{
	// Create the main window for game scene
	if (!InitializeWindow(hInstance, nInitialWinShowState))
		return false;

	// Initialise Input Class
	inputClass = new InputClass();
	if (!inputClass)
		return false;

	// Greate the class that will control all the graphics, If it fails to create, return false
	graphicsClass = new GraphicsClass();
	if (graphicsClass)
		if (!graphicsClass->InitializeGraphics(windowHandle, inputClass))
		{
			MessageBox(NULL, L"Failed to initialize Graphics", L"Error", MB_OK | MB_ICONERROR);
			return false;
		}

	return true;
}

void SystemClass::MessageLoop()
{
	MSG msg = { 0 };
	int count = 0;
	int currentTime = GetTickCount();
	wchar_t fpsText[128];
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Update();
			count++;
			if (GetTickCount() >= currentTime + 1000)
			{
				fps = count;
				count = 0;
				currentTime = GetTickCount();
			}

			swprintf(fpsText, 128, L"FPS = %d", fps);
			SetWindowText(windowHandle, fpsText);


		}
	}
}

void SystemClass::ShutdownSystem()
{
	if (graphicsClass)
		graphicsClass->ShutdownGraphics();

	graphicsClass = 0;
}

LRESULT CALLBACK SystemClass::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		// When the user closes the window by pressing the close button
		// Tell the application to terminate by breaking the windows message loop
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

		// Key Down
	case WM_KEYDOWN:
		inputClass->KeyDown(wParam);
		return 0;

		// Key Up
	case WM_KEYUP:
		inputClass->KeyUp(wParam);
		return 0;

	case WM_MOUSEMOVE:
		mouseXPosition = LOWORD(lParam);
		mouseYPosition = HIWORD(lParam);
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

bool SystemClass::InitializeWindow(HINSTANCE hInstance, int nInitialWinShowState)
{
	// Initialize Window variables
	WNDCLASS windowClass;
	int windowWidth = 1280;
	int windowHeight = 720;
	LPCWSTR windowClassName = L"GameScene";

	// Set Window class Members
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WindowProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = hInstance;
	windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = windowClassName;

	// Register Class, If it doesnt register, throw error
	if (!RegisterClass(&windowClass))
	{
		MessageBox(NULL, L"Unable to register class for application (SystemClass.CPP)", L"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	// Create Window Class
	windowHandle = CreateWindow(windowClassName, L"Game Scene", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, windowWidth, windowHeight, NULL, NULL, hInstance, NULL);

	// Check if Window was created, if false, throw error
	if (!windowHandle)
	{
		MessageBox(NULL, L"Unable to create main window (SystemClass.CPP)", L"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	ShowWindow(windowHandle, nInitialWinShowState);
	return true;
}

void SystemClass::Update()
{
	// Draw The Graphics
	graphicsClass->DrawGraphics();

	// Update the graphics (Rotation of objects etc etc)
	graphicsClass->Update(fps, mouseXPosition, mouseYPosition);
}
