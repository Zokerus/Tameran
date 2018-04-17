#include "IGameWindow.h"
#define IDI_APPICON	108

Hydro::IGameWindow::IGameWindow(HINSTANCE hInst, char *pArgs, const std::string title, const unsigned int screenWidth, const unsigned int screenHeight, bool fullscreen, bool vsync)
	:
	m_appName(title), m_hInst(hInst), m_hWnd(nullptr), m_args(pArgs), m_screenWidth(screenWidth), m_screenHeight(screenHeight), m_fullscreen(fullscreen), m_vsync(vsync), m_ready(false), m_exit(false), m_timer()
{
	//Store window width and height
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	//Convert string to LongPointer Constant CharString
	LPCSTR name = const_cast<char *>(m_appName.c_str());

	//Register Window class
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, HandleMsgSetup, 0, 0, m_hInst, nullptr, nullptr, nullptr, nullptr, name, nullptr };
	wc.hIconSm = (HICON)LoadImage(hInst, MAKEINTRESOURCE(IDI_APPICON), IMAGE_ICON, 16, 16, 0);
	wc.hIcon = (HICON)LoadImage(hInst, MAKEINTRESOURCE(IDI_APPICON), IMAGE_ICON, 32, 32, 0);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	RegisterClassEx(&wc);

	//Create window and get hWnd
	RECT windowRect;

	if (m_fullscreen)
	{
		windowRect.left = 0;
		windowRect.top = 0;
		windowRect.right = windowRect.left + m_screenWidth;
		windowRect.bottom = windowRect.top + m_screenHeight;
		AdjustWindowRect(&windowRect, WS_SYSMENU | WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE, FALSE);
		m_hWnd = CreateWindow(name, name, WS_SYSMENU | WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE, windowRect.left, windowRect.top, m_screenWidth, m_screenHeight, nullptr, nullptr, m_hInst, this);
	
		DEVMODE dmScreenSettings;
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)m_screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)m_screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
	}
	else
	{
		windowRect.left = (GetSystemMetrics(SM_CXSCREEN) - m_screenWidth) / 2;
		windowRect.top = (GetSystemMetrics(SM_CYSCREEN) - m_screenHeight) / 2;
		windowRect.right = windowRect.left + m_screenWidth;
		windowRect.bottom = windowRect.top + m_screenHeight;
		AdjustWindowRect(&windowRect, WS_CAPTION | WS_POPUPWINDOW, FALSE);
		m_hWnd = CreateWindow(name, name, WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE, windowRect.left, windowRect.top, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, nullptr, nullptr, m_hInst, this);
	}

	if (m_hWnd == nullptr)
	{
		ShowMessageBox("Error", "Failed to get valid window handle");
		return;
	}

	// show and update
	ShowWindow(m_hWnd, SW_SHOWDEFAULT);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);
	UpdateWindow(m_hWnd);

	//Get the current working directory
	GetExeDirectory();
}

Hydro::IGameWindow::~IGameWindow()
{
	m_timer.~Timer();

	//Remove the window
	DestroyWindow(m_hWnd);
	m_hWnd = nullptr;

	//Convert string to LongPointer Constant CharString
	LPCSTR name = const_cast<char *>(m_appName.c_str());

	//unregister class
	UnregisterClass(name, m_hInst);
	m_hInst = nullptr;
}

bool Hydro::IGameWindow::IsActive() const
{
	return GetActiveWindow() == m_hWnd;
}

bool Hydro::IGameWindow::IsMinimized() const
{
	return IsIconic(m_hWnd) != 0;;
}

void Hydro::IGameWindow::ShowMessageBox(const std::string & title, const std::string & message) const
{
	//Convert string to LongPointer Constant CharString
	LPCSTR lTitle = const_cast<char *>(title.c_str());
	LPCSTR lMessage = const_cast<char *>(message.c_str());

	MessageBox(m_hWnd, lMessage, lTitle, MB_OK);
}

bool Hydro::IGameWindow::Run()
{
	bool result = false;

	//The game is about to close or the game is not initialized yet
	if (m_exit || !m_ready)
	{
		return false;
	}

	//Update timer object
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

void Hydro::IGameWindow::Kill()
{
	//Kill the window
	PostQuitMessage(0);
}

bool Hydro::IGameWindow::ProcessMessage()
{
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (msg.message == WM_QUIT)
		{
			return false;
		}
	}
	return true;
}

const char* Hydro::IGameWindow::GetArgs() const
{
	return m_args;
}

const HWND Hydro::IGameWindow::GetHandle() const
{
	return m_hWnd;
}

const unsigned int Hydro::IGameWindow::GetWidth() const
{
	return m_screenWidth;
}

const unsigned int Hydro::IGameWindow::GetHeight() const
{
	return m_screenHeight;
}

const bool Hydro::IGameWindow::Fullscreen() const
{
	return m_fullscreen;
}

const bool Hydro::IGameWindow::VSync() const
{
	return m_vsync;
}

LRESULT WINAPI Hydro::IGameWindow::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
	if (msg == WM_NCCREATE)
	{
		// extract ptr to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		IGameWindow* const pWnd = reinterpret_cast<IGameWindow*>(pCreate->lpCreateParams);
		//sanity check
		assert(pWnd != nullptr);
		//set WinAPI-managed user data to store ptr to window class
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		//set message proc to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&IGameWindow::HandleMsgThunk));
		//forward message to window class handler
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	//if we get a message before the WM_NCCREATE message, handle with default handler
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI Hydro::IGameWindow::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//retrieve ptr to window class
	IGameWindow* const pWnd = reinterpret_cast<IGameWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// forward message to window class handler
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Hydro::IGameWindow::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		// Check if the window is being destroyed.
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	// Check if the window is being closed.
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void Hydro::IGameWindow::GetExeDirectory()
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

bool Hydro::IGameWindow::SwitchFullscreen()
{
	if (!m_ready)
		return false;

	if (!m_fullscreen)
	{
		SetWindowLongPtr(m_hWnd, GWL_STYLE,
			WS_SYSMENU | WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE);
		MoveWindow(m_hWnd, 0, 0, m_screenWidth, m_screenHeight, TRUE);

		DEVMODE dmScreenSettings;
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)m_screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)m_screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
		m_fullscreen = true;
	}
	else
	{
		ChangeDisplaySettings(NULL, 0);
		RECT rect;
		rect.left = (GetSystemMetrics(SM_CXSCREEN) - m_screenWidth) / 2;
		rect.top = (GetSystemMetrics(SM_CYSCREEN) - m_screenHeight) / 2;
		rect.right = rect.left + m_screenWidth;
		rect.bottom = rect.top + m_screenHeight;
		SetWindowLongPtr(m_hWnd, GWL_STYLE, WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE);
		AdjustWindowRect(&rect, WS_CAPTION | WS_POPUPWINDOW, FALSE);
		MoveWindow(m_hWnd, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, TRUE);
		m_fullscreen = false;
	}


	return true;
}

