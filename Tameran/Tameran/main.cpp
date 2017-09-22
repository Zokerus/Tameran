#include <GameWindow.h>
#include "Game.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdLine, int iCmdshow)
{
	bool run = false;
	using namespace Hydro;
	using namespace Tameran;

	GameWindow wnd(hInstance, pScmdLine, "Tameran - Hydro Engine", 1024, 768);
	Game game;
	try
	{
		run = game.Initialize(wnd.GetHandle(), hInstance, wnd.GetWidth(), wnd.GetHeight());

		while (wnd.ProcessMessage() &&  run)
		{
			run = game.Run();
		}

	}
	catch (...)
	{
		MessageBox(nullptr, "Error in window operations", "Error", MB_OK);
	}

	//Shutdown game object
	game.Shutdown();

	return 0;
}