#ifndef GAMEWINDOW
#define GAMEWINDOW

//Includes
#include <DirectXColors.h>

//My Includes
#include <IGameWindow.h>
#include <Input/Input.h>
#include <GameStateManager.h>

//Screens
#include "Screens/TitleScreen.h"
#include "Screens/MenuScreen.h"
#include "Screens/LoadingScreen.h"
#include "Screens/GameScreen.h"

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
		~GameWindow();

	private:
		bool Update(float eTime) override;
		bool Draw(float eTime) override;

	private:
		Hydro::Direct3D direct3D;
		Hydro::ShaderManager shaderManager;
		Hydro::Camera camera;
		Hydro::Input input;

	public:
		//GameStates + GameStateManager
		Hydro::GameStateManager gameStateManager;
		TitleScreen titleScreen;
		MenuScreen menuScreen;
		LoadingScreen loadingScreen;
		GameScreen gameScreen;
	};
}

#endif // !GAMEWINDOW

