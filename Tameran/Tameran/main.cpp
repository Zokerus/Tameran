#include "GameWindow.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdLine, int iCmdshow)
{
	bool run = false;
	using namespace Hydro;
	using namespace Tameran;

	
	try
	{
		GameWindow wnd(hInstance, pScmdLine, "Tameran - Hydro Engine", 1024, 768, false, false);
		run = wnd.Initialize();

		while (wnd.ProcessMessage() &&  run)
		{
			run = wnd.Run();
		}

		//Shutdown game object
		wnd.Shutdown();
	}
	catch (std::exception& e)
	{
		MessageBox(nullptr, e.what(), "Error", MB_OK);

	}
	catch (...)
	{
		MessageBox(nullptr, "Error in window operations", "Error", MB_OK);
	}

	return 0;
}