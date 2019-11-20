#include "InputClass.h"


InputClass::InputClass()
{
	for(int i = 0; i < 256; i++)
	{
		KeyStates[i] = false;
	}
}

void InputClass::KeyUp(unsigned keyCode)
{
	KeyStates[keyCode] = false;
}

void InputClass::KeyDown(unsigned KeyCode)
{
	KeyStates[KeyCode] = true;
}

bool InputClass::IsKeyDown(unsigned KeyCode)
{
	return KeyStates[KeyCode];
}

