#include "Timer.h"
#include <exception>

Hydro::Timer::Timer()
	: m_frequency(0.0), m_startTime(0), m_frameTime(0.0), m_beginTime(0), m_endTime(0)
{
	LARGE_INTEGER li;
	//Check to see if the system supports high performance timers
	if (!QueryPerformanceFrequency(&li))
	{
		throw std::exception("System does not support high performance timer. Timer is not initialized.");
	}

	//Calculate the ticks per millisecond
	m_frequency = (double)li.QuadPart;

	//Start the timer
	QueryPerformanceCounter(&li);
	m_startTime = li.QuadPart;
}

Hydro::Timer::~Timer()
{}

const bool Hydro::Timer::Update()
{
	LARGE_INTEGER li;
	//Get the latest timing
	QueryPerformanceCounter(&li);
	//Calculate the frame time
	m_frameTime = (double)(li.QuadPart - m_startTime) / m_frequency;
	//Reset the timer
	m_startTime = li.QuadPart;
	return true;
}

float Hydro::Timer::GetTime() const
{
	return (float)m_frameTime;
}
