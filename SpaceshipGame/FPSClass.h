#pragma once

class FPSClass
{
public:
	FPSClass();
	FPSClass(const FPSClass&);
	~FPSClass();

	void Initialize();
	void Frame();
	int GetFPS();

private:
	int m_FPS = 0;
	int m_Count = 0;
	unsigned long m_StartTime = 0;
};