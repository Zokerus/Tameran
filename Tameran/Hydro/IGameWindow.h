#ifndef IGAMEWINDOWHEADER
#define IGAMEWINDOWHEADER

//Includes
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>

//My Include
#include "Util/Globals.h"
#include "Util/Timer.h"
#include "Graphics/Direct3D.h"

//////////////////////////////////////////////////////////////////////////
//Class: IGameWindow
//////////////////////////////////////////////////////////////////////////
namespace Hydro
{
	class IGameWindow
	{
	public:
		IGameWindow(HINSTANCE hInst, char *pArgs, const std::string title, const unsigned int screenWidth, const unsigned int screenHeight, bool fullscreen, bool vsync);
		IGameWindow() = delete;
		IGameWindow(const IGameWindow &other) = delete;
		IGameWindow& operator=(const IGameWindow &other) = delete;
		~IGameWindow();

		bool Initialize();
		void Shutdown();

		bool IsActive() const;
		bool IsMinimized() const;
		void ShowMessageBox(const std::string &title, const std::string &message) const;
		bool Run();
		void Kill();
		bool ProcessMessage();
		const char* GetArgs() const;
		const HWND GetHandle() const;

		const unsigned int GetWidth() const;
		const unsigned int GetHeight() const;
		const bool Fullscreen() const;
		const bool VSync() const;

	private:
		static LRESULT WINAPI HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT WINAPI HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		void GetExeDirectory();

	protected:
		virtual bool Update(float eTime) = 0;
		virtual bool Draw(float eTime) = 0;

	protected:
		std::string m_appName;
		HINSTANCE m_hInst;
		HWND m_hWnd;
		char *m_args;
		unsigned int m_screenWidth, m_screenHeight;
		bool m_fullscreen, m_vsync;
		float m_screenNear, m_screenDepth;
		bool m_ready;
		bool m_exit;

		Timer m_timer;
		Direct3D m_direct3D;
	};
}

#endif // !GAMEWINDOWHEADER

