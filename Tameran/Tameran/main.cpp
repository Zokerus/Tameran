#include "GameWindow.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdLine, int iCmdshow)
{
	bool run = false;
	using namespace Hydro;
	using namespace Tameran;

	GameWindow wnd(hInstance, pScmdLine, "Tameran - Hydro Engine", 1024, 768, true, false);
	try
	{
		run = wnd.Initialize();

		while (wnd.ProcessMessage() &&  run)
		{
			run = wnd.Run();
		}

	}
	catch (...)
	{
		MessageBox(nullptr, "Error in window operations", "Error", MB_OK);
	}

	//Shutdown game object
	wnd.Shutdown();

	return 0;
}