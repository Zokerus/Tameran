#include "GameWindow.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdLine, int iCmdshow)
{
	bool run = true;
	using namespace Hydro;
	using namespace Tameran;

	
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
		MessageBox(nullptr, e.what(), "Error", MB_OK);
	}
	catch (...)
	{
		MessageBox(nullptr, "Error in window operations", "Error", MB_OK);
	}

	return 0;
}