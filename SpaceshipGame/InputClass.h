#pragma once

class InputClass
{
public:
	InputClass();
	InputClass(const InputClass& other);
	~InputClass();

	HRESULT Initialize(const HINSTANCE& const hinstance, const HWND& const hwnd, const int& const ScreenWidth, const int& const ScreenHeight);
	void Shutdown();
	HRESULT Frame();

	// Getter //

	inline bool IsEscapePressed() { return ((m_KeyboardState[DIK_ESCAPE] & 0x80) ? true : false); }
	inline bool IsLeftArrowPressed() { return ((m_KeyboardState[DIK_LEFTARROW] & 0x80) ? true : false); }
	inline bool IsRightArrowPressed() { return ((m_KeyboardState[DIK_RIGHTARROW] & 0x80) ? true : false); }

	// Setter //

	inline void GetMouseLocation(int& MouseX, int& MouseY) { MouseX = m_MouseX; MouseY = m_MouseY; }

private:
	HRESULT ReadKeyboard();
	HRESULT ReadMouse();
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