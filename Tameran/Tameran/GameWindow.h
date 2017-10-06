#ifndef GAMEWINDOW
#define GAMEWINDOW

//My Includes
#include <IGameWindow.h>

//////////////////////////////////////////////////////////////////////////
//Class: Game
//////////////////////////////////////////////////////////////////////////
namespace Tameran
{
	class GameWindow : public Hydro::IGameWindow
	{
	public:
		GameWindow(HINSTANCE hInst, char *pArgs);
		GameWindow(HINSTANCE hInst, char *pArgs, const std::string title);
		GameWindow(HINSTANCE hInst, char *pArgs, const unsigned int screenWidth, const unsigned int screenHeight);
		GameWindow(HINSTANCE hInst, char *pArgs, const std::string title, const unsigned int screenWidth, const unsigned int screenHeight);
		GameWindow(HINSTANCE hInst, char *pArgs, const std::string title, const unsigned int screenWidth, const unsigned int screenHeight, bool fullscreen, bool vsync);
		GameWindow() = delete;
		GameWindow(const GameWindow &other) = delete;
		GameWindow& operator=(const GameWindow &other) = delete;
		~GameWindow();

		bool Initialize();
		void Shutdown();

	private:
		bool Update(float eTime);
		bool Draw(float eTime);
	};
}

#endif // !GAMEWINDOW

