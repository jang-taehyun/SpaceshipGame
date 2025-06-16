#pragma once

class FPSClass
{
public:
	FPSClass();
	virtual ~FPSClass();

	void Frame();
	
	// Getter //

	inline int GetFPS() const { return m_FPS; }

private:
	void Initialize();

private:
	static bool IsInitailize;

	int m_FPS = 0;
	int m_Count = 0;
	unsigned long m_StartTime = 0;

public:
	FPSClass(const FPSClass& other) = delete;
	FPSClass(FPSClass&& other) = delete;
};