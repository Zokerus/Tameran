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
		Timer(const Timer &other) = delete;
		Timer& operator=(Timer &other) = delete;
		~Timer();

		const bool Initialize();
		const bool Update();

		float GetTime() const;

	private:
		bool m_ready;
		double m_frequency;
		INT64 m_startTime;
		double m_frameTime;
		INT64 m_beginTime, m_endTime;
	};
}

#endif // !TIMERHEADER
