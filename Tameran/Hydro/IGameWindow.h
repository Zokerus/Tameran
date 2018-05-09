#ifndef IGAMEWINDOWHEADER
#define IGAMEWINDOWHEADER

//Includes
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>

//My Include
#include "Util/Timer.h"
#include "Graphics/Direct3D.h"
#include "Graphics/Shader/ShaderManager.h"
#include "Graphics/Camera.h"

//////////////////////////////////////////////////////////////////////////
//Class: IGameWindow
//////////////////////////////////////////////////////////////////////////
namespace Hydro
{
	class IGameWindow
	{
	public:
		IGameWindow(HINSTANCE _hInst, char* pArgs, const std::string title, const unsigned int _screenWidth, const unsigned int _screenHeight, bool _fullscreen, bool _vSync);
		IGameWindow() = delete;
		IGameWindow(const IGameWindow &other) = delete;
		IGameWindow& operator=(const IGameWindow &other) = delete;
		~IGameWindow();

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

		//void GetExeDirectory();

	protected:
		virtual bool Update(float eTime) = 0;
		virtual bool Draw(float eTime) = 0;

		bool SwitchFullscreen();

	protected:
		std::string appName;
		HINSTANCE hInst;
		HWND hWnd;
		char* args;
		unsigned int screenWidth, screenHeight;
		bool fullscreen, vSync;
		float screenNear, screenDepth;
		//bool ready;
		bool exit;

		Timer timer;
	};
}

#endif // !GAMEWINDOWHEADER

