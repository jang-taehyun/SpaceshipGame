#pragma once

class InputClass
{
public:
	InputClass(const HINSTANCE& hinstance, const HWND& hwnd, const int& ScreenWidth, const int& ScreenHeight);
	~InputClass();
	
	HRESULT Frame();

	// Getter //

	inline bool IsEscapePressed() const { return ((m_KeyboardState[DIK_ESCAPE] & 0x80) ? true : false); }
	inline bool IsLeftArrowPressed() const { return ((m_KeyboardState[DIK_LEFTARROW] & 0x80) ? true : false); }
	inline bool IsRightArrowPressed() const { return ((m_KeyboardState[DIK_RIGHTARROW] & 0x80) ? true : false); }

	// Setter //

	inline void GetMouseLocation(int& MouseX, int& MouseY) const { MouseX = m_MouseX; MouseY = m_MouseY; }

private:
	HRESULT Initialize(const HINSTANCE& hinstance, const HWND& hwnd, const int& ScreenWidth, const int& ScreenHeight);
	void Shutdown();

	HRESULT ReadKeyboard();
	HRESULT ReadMouse();
	void ProcessInput();

private:
	static bool IsInitailize;

	IDirectInput8* m_DirectInput = nullptr;
	IDirectInputDevice8* m_Keyboard = nullptr;
	IDirectInputDevice8* m_Mouse = nullptr;

	unsigned char m_KeyboardState[256] = { 0, };
	DIMOUSESTATE m_MouseState;

	int m_ScreenWidth = 0;
	int m_ScreenHeight = 0;
	int m_MouseX = 0;
	int m_MouseY = 0;

public:
	InputClass() = delete;
	InputClass(const InputClass& other) = delete;
};