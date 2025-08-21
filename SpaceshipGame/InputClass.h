#pragma once

namespace System
{
	class InputClass
	{
	private:
		enum MOUSEBUTTON
		{
			MOUSE_LEFT = 0, MOUSE_RIGHT = 1, MOUSE_CENTER = 2,
		};

	public:
		InputClass(HINSTANCE hinstance, HWND hwnd, int ScreenWidth, int ScreenHeight);
		virtual ~InputClass();

		void Frame(HWND hwnd);

		bool IsEscapePressed() const { return m_KeyboardState[DIK_ESCAPE] & 0x80; }

		bool IsLeftArrowPressed() const { return m_KeyboardState[DIK_LEFTARROW] & 0x80; }
		bool IsRightArrowPressed() const { return m_KeyboardState[DIK_RIGHTARROW] & 0x80; }
		bool IsUpArrowPressed() const { return m_KeyboardState[DIK_UPARROW] & 0x80; }
		bool IsDownArrowPressed() const { return m_KeyboardState[DIK_DOWNARROW] & 0x80; }

		bool IsABottunPressed() const { return m_KeyboardState[DIK_A] & 0x80; }
		bool IsSBottunPressed() const { return m_KeyboardState[DIK_S] & 0x80; }
		bool IsWBottunPressed() const { return m_KeyboardState[DIK_W] & 0x80; }
		bool IsDBottunPressed() const { return m_KeyboardState[DIK_D] & 0x80; }

		bool IsSpacebarPressed() const { return m_KeyboardState[DIK_SPACE] & 0x80; }

		bool IsMouseCenterBottunPressed() const { return m_MouseState.rgbButtons[MOUSEBUTTON::MOUSE_CENTER] & 0x80; }
		bool IsMouseLeftBottunPressed() const { return m_MouseState.rgbButtons[MOUSEBUTTON::MOUSE_LEFT] & 0x80; }

		void GetMouseLocation(int& MouseX, int& MouseY) const { MouseX = m_MousePos.x; MouseY = m_MousePos.y; }
		void GetMouseMoveDelta(long& MouseX, long& MouseY) const { MouseX = m_MouseState.lX; MouseY = m_MouseState.lY; }

	private:
		HRESULT Initialize(HINSTANCE hinstance, HWND hwnd, int ScreenWidth, int ScreenHeight);
		void Shutdown();

		HRESULT ReadKeyboard();
		HRESULT ReadMouse();
		void ProcessInput(HWND hwnd);

	private:
		static bool IsInitailize;

		Microsoft::WRL::ComPtr<IDirectInput8> m_DirectInput = nullptr;
		Microsoft::WRL::ComPtr<IDirectInputDevice8> m_Keyboard = nullptr;
		Microsoft::WRL::ComPtr<IDirectInputDevice8> m_Mouse = nullptr;

		unsigned char m_KeyboardState[256] = { 0, };
		DIMOUSESTATE m_MouseState = {};

		POINT m_MousePos = {};

	public:
		InputClass(const InputClass& other) = delete;
		InputClass(InputClass&& other) = delete;
		InputClass& operator=(const InputClass& other) = delete;
		InputClass& operator=(InputClass&& other) = delete;
	};
}