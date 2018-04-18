#ifndef TIMERHEADER
#define TIMERHEADER

//Includes
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

//////////////////////////////////////////////////////////////////////////
//Class: Timer
//////////////////////////////////////////////////////////////////////////
namespace Hydro
{
	class Timer
	{
	public:
		Timer();
		~Timer();

		const bool Update();

		float GetTime() const;

	private:
		double frequency;
		INT64 startTime;
		double frameTime;
		INT64 beginTime, endTime;
	};
}

#endif // !TIMERHEADER
