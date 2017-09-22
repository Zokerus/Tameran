#ifndef GAMEWINDOWHEADER
#define GAMEWINDOWHEADER

//Includes
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>

//////////////////////////////////////////////////////////////////////////
//Class: GameWindow
//////////////////////////////////////////////////////////////////////////
namespace Hydro
{
	class GameWindow
	{
	public:
		GameWindow(HINSTANCE hInst, char *pArgs);
		GameWindow(HINSTANCE hInst, char *pArgs, const std::string title);
		GameWindow(HINSTANCE hInst, char *pArgs, const unsigned int screenWidth, const unsigned int screenHeight);
		GameWindow(HINSTANCE hInst, char *pArgs, const std::string title, const unsigned int screenWidth, const unsigned int screenHeight);
		GameWindow() = delete;
		GameWindow(const GameWindow &other) = delete;
		GameWindow& operator=(const GameWindow &other) = delete;
		~GameWindow();

		bool IsActive() const;
		bool IsMinimized() const;
		void ShowMessageBox(const std::string &title, const std::string &message) const;
		void Kill();
		bool ProcessMessage();
		const char* GetArgs() const;
		const HWND GetHandle() const;

		const unsigned int GetWidth() const;
		const unsigned int GetHeight() const;

	private:
		static LRESULT WINAPI HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT WINAPI HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	private:
		std::string m_appName;
		HINSTANCE m_hInst;
		HWND m_hWnd;
		char *m_args;
		unsigned int m_screenWidth, m_screenHeight;
		static constexpr bool m_fullscreen = false;
	};
}

#endif // !GAMEWINDOWHEADER

