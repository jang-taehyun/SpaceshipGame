#pragma once

class FPSClass
{
public:
	FPSClass();
	~FPSClass();

	void Frame();
	
	// Getter //

	inline const int& GetFPS() const { return m_FPS; }

private:
	HRESULT Initialize();

private:
	static bool IsInitailize;

	int m_FPS = 0;
	int m_Count = 0;
	unsigned long m_StartTime = 0;

public:
	FPSClass(const FPSClass& other) = delete;
};