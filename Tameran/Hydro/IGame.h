#ifndef IGAMEHEADER
#define IGAMEHEADER

//Includes
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

//My Include
#include "Util/Globals.h"
#include "Util/Timer.h"

//////////////////////////////////////////////////////////////////////////
//Class: IGame
//////////////////////////////////////////////////////////////////////////
namespace Hydro
{
	class IGame
	{
	public:
		IGame();
		IGame(const IGame &other) = delete;
		IGame& operator=(const IGame &other) = delete;
		~IGame();

		bool Initialize(HWND hWnd, HINSTANCE hInst, unsigned int screenWidth, unsigned int screenHeight);
		void Shutdown();
		bool Run();
		void Exit();

	protected:
		virtual bool Update(float eTime) = 0;
		virtual bool Draw(float eTime) = 0;

	private:
		void GetExeDirectory();

	protected:
		HWND m_hWnd;
		HINSTANCE m_hInst;
		unsigned int m_screenWidth, m_screenHeight;
		bool m_fullscreen, m_vsync;
		float m_screenNear, m_screenDepth;
		bool m_ready;
		bool m_exit;

		Timer m_timer;
		//Direct3D *m_direct3D;
	};
}
#endif // !IGAMEHEADER

