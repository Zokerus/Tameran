#include "GameWindow.h"
#include <assert.h>
#define IDI_APPICON	108

Hydro::GameWindow::GameWindow(HINSTANCE hInst, char *pArgs)
	: GameWindow(hInst, pArgs, "DirectX Window", 1024, 768)
{}

Hydro::GameWindow::GameWindow(HINSTANCE hInst, char *pArgs, const std::string title)
	: GameWindow(hInst, pArgs, title, 1024, 768)
{}

Hydro::GameWindow::GameWindow(HINSTANCE hInst, char *pArgs, const unsigned int screenWidth, const unsigned int screenHeight)
	: GameWindow(hInst, pArgs, "DirectX Window", screenWidth, screenHeight)
{}

Hydro::GameWindow::GameWindow(HINSTANCE hInst, char *pArgs, const std::string title, const unsigned int screenWidth, const unsigned int screenHeight)
	:
	m_appName(title), m_hInst(hInst), m_hWnd(nullptr), m_args(pArgs), m_screenWidth(0), m_screenHeight(0)
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
		AdjustWindowRect(&windowRect, 0, FALSE);
		m_hWnd = CreateWindow(name, name, 0, windowRect.left, windowRect.top, m_screenWidth, m_screenHeight, nullptr, nullptr, m_hInst, this);
	}
	else
	{

		windowRect.left = (GetSystemMetrics(SM_CXSCREEN) - m_screenWidth) / 2;
		windowRect.top = (GetSystemMetrics(SM_CYSCREEN) - m_screenHeight) / 2;
		windowRect.right = windowRect.left + m_screenWidth;
		windowRect.bottom = windowRect.top + m_screenHeight;
		AdjustWindowRect(&windowRect, WS_SYSMENU | WS_CAPTION, FALSE);
		m_hWnd = CreateWindow(name, name, WS_SYSMENU | WS_CAPTION, windowRect.left, windowRect.top, m_screenWidth, m_screenHeight, nullptr, nullptr, m_hInst, this);
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
}

Hydro::GameWindow::~GameWindow()
{
	//Remove the window
	DestroyWindow(m_hWnd);
	m_hWnd = nullptr;

	//Convert string to LongPointer Constant CharString
	LPCSTR name = const_cast<char *>(m_appName.c_str());

	//unregister class
	UnregisterClass(name, m_hInst);
	m_hInst = nullptr;
}

bool Hydro::GameWindow::IsActive() const
{
	return GetActiveWindow() == m_hWnd;
}

bool Hydro::GameWindow::IsMinimized() const
{
	return IsIconic(m_hWnd) != 0;;
}

void Hydro::GameWindow::ShowMessageBox(const std::string & title, const std::string & message) const
{
	//Convert string to LongPointer Constant CharString
	LPCSTR lTitle = const_cast<char *>(title.c_str());
	LPCSTR lMessage = const_cast<char *>(message.c_str());

	MessageBox(m_hWnd, lMessage, lTitle, MB_OK);
}

void Hydro::GameWindow::Kill()
{
	//Kill the window
	PostQuitMessage(0);
}

bool Hydro::GameWindow::ProcessMessage()
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

const char* Hydro::GameWindow::GetArgs() const
{
	return m_args;
}

const HWND Hydro::GameWindow::GetHandle() const
{
	return m_hWnd;
}

const unsigned int Hydro::GameWindow::GetWidth() const
{
	return m_screenWidth;
}

const unsigned int Hydro::GameWindow::GetHeight() const
{
	return m_screenHeight;
}

LRESULT Hydro::GameWindow::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
	if (msg == WM_NCCREATE)
	{
		// extract ptr to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		GameWindow* const pWnd = reinterpret_cast<GameWindow*>(pCreate->lpCreateParams);
		//sanity check
		assert(pWnd != nullptr);
		//set WinAPI-managed user data to store ptr to window class
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		//set message proc to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&GameWindow::HandleMsgThunk));
		//forward message to window class handler
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	//if we get a message before the WM_NCCREATE message, handle with default handler
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Hydro::GameWindow::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//retrieve ptr to window class
	GameWindow* const pWnd = reinterpret_cast<GameWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// forward message to window class handler
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Hydro::GameWindow::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
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

