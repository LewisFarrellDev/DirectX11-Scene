#include <Windows.h>
#include "GraphicsClass.h"
#include "InputClass.h"

class SystemClass
{
public:
	SystemClass();
	bool InitializeSystem(HINSTANCE hInstance, int nInitialWinShowState);
	void MessageLoop();
	void ShutdownSystem();
	HWND windowHandle;

private:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool InitializeWindow(HINSTANCE hInstance, int nInitialWinShowState);
	GraphicsClass *graphicsClass = nullptr;
	void Update();
	int fps = 0;
};

// Globals
static InputClass *inputClass = nullptr;
static int mouseXPosition = 0;
static int mouseYPosition = 0;