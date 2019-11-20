
class InputClass
{
public:
	InputClass();

	void KeyUp(unsigned keyCode);
	void KeyDown(unsigned KeyCode);
	bool IsKeyDown(unsigned KeyCode);

private:
	bool KeyStates[256];
};

