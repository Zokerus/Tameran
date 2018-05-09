#include "GameWindow.h"
#include "Character/StatManager.h"

#include <fstream>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdLine, int iCmdshow)
{
	bool run = true;
	using namespace Hydro;
	using namespace Tameran;

	StatManager manager;

	try
	{
		GameWindow wnd(hInstance, pScmdLine, "Tameran - Hydro Engine", 1024, 768, false, false);

		while (wnd.ProcessMessage() && run)
		{
			run = wnd.Run();
		}
	}
	catch (const std::exception& e)
	{
		MessageBox(0, e.what(), "Error", MB_OK);
		//Not Showing
	}
	catch (...)
	{
		MessageBox(0, "Error in window operations", "Error", MB_OK);
		//Not Showing
	}

	return 0;
}