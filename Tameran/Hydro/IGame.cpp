#include "IGame.h"

Hydro::IGame::IGame()
	:
	m_hWnd(nullptr), m_hInst(nullptr), m_screenWidth(0), m_screenHeight(0), m_fullscreen(false), m_vsync(false), m_screenNear(0.0f), m_screenDepth(0.0f), m_ready(false), m_exit(false),
	m_timer()
{}

Hydro::IGame::~IGame()
{
	if (m_ready)
	{
		Shutdown();
	}
}

bool Hydro::IGame::Initialize(HWND hWnd, HINSTANCE hInst, unsigned int screenWidth, unsigned int screenHeight)
{
	bool result = false;

	//Get the working directory
	GetExeDirectory();

	//Store the handle and instance of the window
	m_hWnd = hWnd;
	m_hInst = hInst;

	//Store the preset screen width and screen height
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	//Store the vsync value and screen parameters
	m_vsync = false;
	m_screenNear = 0.1f;
	m_screenDepth = 1000.0f;

	//Initialize the timer object
	result = m_timer.Initialize();
	if (!result)
	{
		MessageBox(m_hWnd, "Could not initialize the timer object.", "Error", MB_OK);
		return false;
	}

	//m_ready not set to true, because you need to initialize the child class afterwards and after that set m_ready
	return true;
}

void Hydro::IGame::Shutdown()
{
}

bool Hydro::IGame::Run()
{
	bool result = false;

	//The game is about to close or the game is not initialized yet
	if(m_exit || !m_ready)
	{
		return false;
	}

	//Update the timer
	result = m_timer.Update();
	if (!result)
	{
		return false;
	}

	//Do the frame processing
	result = Update(m_timer.GetTime());
	if (!result)
	{
		return false;
	}

	//Render the new scene
	result = Draw(m_timer.GetTime());
	if (!result)
	{
		return false;
	}

	return true;
}

void Hydro::IGame::Exit()
{
	m_exit = true;
}

void Hydro::IGame::GetExeDirectory()
{
	//Get the path of executable
	int indexSlash;
	WCHAR path[MAX_PATH];
	char sPath[MAX_PATH];

	HMODULE hModule = GetModuleHandleW(NULL);
	GetModuleFileName(hModule, sPath, MAX_PATH);

	for (int i = 0; i < MAX_PATH; i++)
	{
		char letter = sPath[i];
		if (letter == 92)
		{
			indexSlash = i;
			sPath[i] = '/';
		}
		else if (letter == '\0')
		{
			break;
		}
		path[i] = sPath[i];
	}

	wDir = ((std::wstring)path).substr(0, indexSlash + 1);
	sDir = ((std::string)sPath).substr(0, indexSlash + 1);
}
