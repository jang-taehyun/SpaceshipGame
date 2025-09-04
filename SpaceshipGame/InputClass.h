#pragma once

namespace System
{
	class InputClass
	{
	private:

		struct KeyState
		{
			KEYSTATE CurrentState = KEYSTATE::NONE;
			bool PreviousPush = false;
		};

	public:
		InputClass(int ScreenWidth, int ScreenHeight);
		virtual ~InputClass();

		void Frame();

		KEYSTATE GetKeyState(KEY key) const;

		void GetMouseLocation(int& MouseX, int& MouseY) const { MouseX = m_MousePos.x; MouseY = m_MousePos.y; }
		void GetMouseMoveDelta(long& MouseX, long& MouseY) const { MouseX = m_MouseState.lX; MouseY = m_MouseState.lY; }

	private:
		HRESULT Initialize(int ScreenWidth, int ScreenHeight);
		void Shutdown();

		HRESULT ReadKeyboard();
		HRESULT ReadMouse();
		void ProcessInput();

		void UpdateMouseButton(UINT idx);
		void UpdateKeyboardButton(UINT idx);

	private:
		static bool IsInitailize;

		Microsoft::WRL::ComPtr<IDirectInput8> m_DirectInput = nullptr;
		Microsoft::WRL::ComPtr<IDirectInputDevice8> m_Keyboard = nullptr;
		Microsoft::WRL::ComPtr<IDirectInputDevice8> m_Mouse = nullptr;

		unsigned char m_KeyboardState[256] = { 0, };
		DIMOUSESTATE m_MouseState = {};

		KeyState m_State[static_cast<UINT>(System::KEY::LAST)] = {};
		POINT m_MousePos = {};

	public:
		InputClass(const InputClass& other) = delete;
		InputClass(InputClass&& other) = delete;
		InputClass& operator=(const InputClass& other) = delete;
		InputClass& operator=(InputClass&& other) = delete;
	};
}