#include <GameWindow.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdLine, int iCmdshow)
{
	bool done = false;
	using namespace Hydro;

	GameWindow wnd(hInstance, pScmdLine, "Tameran - Hydro Engine", 1024, 768);

	try
	{
		while (wnd.ProcessMessage())
		{
		}

	}
	catch (...)
	{
		MessageBox(nullptr, "Error in window operations", "Error", MB_OK);
	}

	return 0;
}