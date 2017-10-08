#include "Timer.h"

Hydro::Timer::Timer()
	: m_ready(false), m_frequency(0.0), m_startTime(0), m_frameTime(0.0), m_beginTime(0), m_endTime(0)
{}

Hydro::Timer::~Timer()
{}

const bool Hydro::Timer::Initialize()
{
	LARGE_INTEGER li;
	//Check to see if the system supports high performance timers
	if (!QueryPerformanceFrequency(&li))
	{
		return false;
	}

	//Calculate the ticks per millisecond
	m_frequency = (double)li.QuadPart;

	//Start the timer
	QueryPerformanceCounter(&li);
	m_startTime = li.QuadPart;

	//Flag the class as ready
	m_ready = true;
	return true;
}

const bool Hydro::Timer::Update()
{
	if (!m_ready)
	{
		//Check if the class is ready
		return false;
	}

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
