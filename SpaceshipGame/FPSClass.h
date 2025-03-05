#pragma once

class FPSClass
{
public:
	FPSClass();
	FPSClass(const FPSClass& other);
	~FPSClass();

	void Initialize();
	void Frame();
	
	// Getter //

	inline const int& GetFPS() const { return m_FPS; }

private:
	int m_FPS = 0;
	int m_Count = 0;
	unsigned long m_StartTime = 0;
};