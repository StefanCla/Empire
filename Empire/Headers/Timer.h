#pragma once
#include "Headers/Precomp.h"
#include <chrono>

class Timer
{
public:
	Timer(const Timer& t) = delete;
	Timer& operator=(const Timer& t) = delete;

	static Timer* Instance();
	static void Release();

	void Reset();
	float DeltaTime();
	void TimeScale(float t = 1.0f);
	float TimeScale();
	void Tick();

private:
	Timer();
	~Timer();

	static Timer* s_Instance;

	std::chrono::system_clock::time_point m_StartTime;
	std::chrono::duration<float> m_DeltaTime;
	float m_TimeScale;
};
