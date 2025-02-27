#pragma once

class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

	bool IsEscapePressed();
	bool IsLeftArrowPressed();
	bool IsRightArrowPressed();
	void GetMouseLocation(int&, int&);

private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

private:
	IDirectInput8* m_DirectInput = nullptr;
	IDirectInputDevice8* m_Keyboard = nullptr;
	IDirectInputDevice8* m_Mouse = nullptr;

	unsigned char m_KeyboardState[256] = { 0, };
	DIMOUSESTATE m_MouseState;

	int m_ScreenWidth = 0;
	int m_ScreenHeight = 0;
	int m_MouseX = 0;
	int m_MouseY = 0;
};