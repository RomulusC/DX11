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
		return std::chrono::duration<double>(std::chrono::system_clock::now() - m_timeStart).count();
	}
	void Reset()
	{
		m_timeStart = std::chrono::system_clock::now();
	}
private:
	std::chrono::time_point<std::chrono::system_clock> m_timeStart;
};