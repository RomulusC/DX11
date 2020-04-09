#pragma once
#include <chrono>

class Timer
{
public:
	Timer()
		:
		m_timeStart(std::chrono::system_clock::now())
	{}
	float Peak()
	{
		return std::chrono::duration<float>(std::chrono::system_clock::now() - m_timeStart).count();
	}
	void Reset()
	{
		m_timeStart = std::chrono::system_clock::now();
	}
	float Mark()
	{
		auto old = m_timeStart;
		m_timeStart = std::chrono::system_clock::now();
		return std::chrono::duration<float>(m_timeStart - old).count();
	}
private:
	std::chrono::time_point<std::chrono::system_clock> m_timeStart;
};