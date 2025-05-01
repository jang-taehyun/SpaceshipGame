#pragma once

class InputClass
{
private:
	enum MOUSEBUTTON
	{
		MOUSE_LEFT = 0, MOUSE_RIGHT = 1, MOUSE_CENTER = 2,
	};

public:
	explicit InputClass(const HINSTANCE& hinstance, const HWND& hwnd, const int& ScreenWidth, const int& ScreenHeight);
	virtual ~InputClass();
	
	HRESULT Frame();

	// Getter //

	inline bool IsEscapePressed() const { return m_KeyboardState[DIK_ESCAPE] & 0x80; }

	inline bool IsLeftArrowPressed() const { return m_KeyboardState[DIK_LEFTARROW] & 0x80; }
	inline bool IsRightArrowPressed() const { return m_KeyboardState[DIK_RIGHTARROW] & 0x80; }
	inline bool IsUpArrowPressed() const { return m_KeyboardState[DIK_UPARROW] & 0x80; }
	inline bool IsDownArrowPressed() const { return m_KeyboardState[DIK_DOWNARROW] & 0x80; }

	inline bool IsABottunPressed() const { return m_KeyboardState[DIK_A] & 0x80; }
	inline bool IsSBottunPressed() const { return m_KeyboardState[DIK_S] & 0x80; }
	inline bool IsWBottunPressed() const { return m_KeyboardState[DIK_W] & 0x80; }
	inline bool IsDBottunPressed() const { return m_KeyboardState[DIK_D] & 0x80; }

	inline bool IsSpacebarPressed() const { return m_KeyboardState[DIK_SPACE] & 0x80; }

	inline bool IsMouseCenterBottunPressed() const { return m_MouseState.rgbButtons[MOUSEBUTTON::MOUSE_CENTER] & 0x80; }

	// Setter //

	inline void GetMouseLocation(int& MouseX, int& MouseY) const { MouseX = m_MouseX; MouseY = m_MouseY; }
	inline void GetMouseMoveDelta(long& MouseX, long& MouseY) const { MouseX = m_MouseState.lX; MouseY = m_MouseState.lY; }

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