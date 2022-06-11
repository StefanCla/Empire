#include "Headers/Timer.h"

Timer* Timer::s_Instance = nullptr;

Timer* Timer::Instance()
{
	if (s_Instance == nullptr)
		s_Instance = new Timer();

	return s_Instance;
}

void Timer::Release()
{
	delete s_Instance;
	s_Instance = nullptr;
}

Timer::Timer()
{
	Reset();
	m_TimeScale = 1.0f;
	m_DeltaTime = std::chrono::duration<float>(0.0f);
}

Timer::~Timer()
{}

void Timer::Reset()
{
	m_StartTime = std::chrono::system_clock::now();
}

float Timer::DeltaTime()
{
	return m_DeltaTime.count();
}

void Timer::TimeScale(float t)
{
	m_TimeScale = t;
}

float Timer::TimeScale()
{
	return m_TimeScale;
}

void Timer::Tick()
{
	m_DeltaTime = std::chrono::system_clock::now() - m_StartTime;
}
