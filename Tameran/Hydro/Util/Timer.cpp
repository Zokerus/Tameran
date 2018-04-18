#include "Timer.h"
#include <exception>

Hydro::Timer::Timer()
	: frequency(0.0), startTime(0), frameTime(0.0), beginTime(0), endTime(0)
{
	LARGE_INTEGER li;
	//Check to see if the system supports high performance timers
	if (!QueryPerformanceFrequency(&li))
	{
		throw std::exception("System does not support high performance timer. Timer is not initialized.");
	}

	//Calculate the ticks per millisecond
	frequency = (double)li.QuadPart;

	//Start the timer
	QueryPerformanceCounter(&li);
	startTime = li.QuadPart;
}

Hydro::Timer::~Timer()
{}

const bool Hydro::Timer::Update()
{
	LARGE_INTEGER li;
	//Get the latest timing
	QueryPerformanceCounter(&li);
	//Calculate the frame time
	frameTime = (double)(li.QuadPart - startTime) / frequency;
	//Reset the timer
	startTime = li.QuadPart;
	return true;
}

float Hydro::Timer::GetTime() const
{
	return (float)frameTime;
}
