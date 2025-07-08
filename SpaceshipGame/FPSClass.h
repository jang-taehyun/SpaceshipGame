#pragma once

namespace System
{
	class FPSClass
	{
	public:
		FPSClass();
		virtual ~FPSClass();

		void Frame();

		inline UINT GetFPS() const { return m_FPS; }

	private:
		void Initialize();

	private:
		static bool IsInitailize;

		UINT m_FPS = 0;
		UINT m_Count = 0;
		ULONG m_StartTime = 0;

	public:
		FPSClass(const FPSClass& other) = delete;
		FPSClass(FPSClass&& other) = delete;
		FPSClass& operator=(const FPSClass& other) = delete;
		FPSClass& operator=(FPSClass&& other) = delete;
	};
}
