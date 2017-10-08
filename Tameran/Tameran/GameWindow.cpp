#include "GameWindow.h"

Tameran::GameWindow::GameWindow(HINSTANCE hInst, char * pArgs)
	: GameWindow(hInst, pArgs, "DirectX Window", 1024, 768, false, false)
{}

Tameran::GameWindow::GameWindow(HINSTANCE hInst, char * pArgs, const std::string title)
	: GameWindow(hInst, pArgs, title, 1024, 768, false, false)
{}

Tameran::GameWindow::GameWindow(HINSTANCE hInst, char * pArgs, const unsigned int screenWidth, const unsigned int screenHeight)
	: GameWindow(hInst, pArgs, "DirectX Window", screenWidth, screenHeight, false, false)
{}

Tameran::GameWindow::GameWindow(HINSTANCE hInst, char * pArgs, const std::string title, const unsigned int screenWidth, const unsigned int screenHeight)
	: GameWindow(hInst, pArgs, title, screenWidth, screenHeight, false, false)
{}

Tameran::GameWindow::GameWindow(HINSTANCE hInst, char * pArgs, const std::string title, const unsigned int screenWidth, const unsigned int screenHeight, bool fullscreen, bool vsync)
	: IGameWindow(hInst, pArgs, title, screenWidth, screenHeight, fullscreen, vsync)
{
}

Tameran::GameWindow::~GameWindow()
{
	if (m_ready)
	{
		Shutdown();
	}
}

bool Tameran::GameWindow::Initialize()
{
	bool result = false;

	//Initialize the parent class
	result = Hydro::IGameWindow::Initialize();

	m_ready = result;
	return m_ready;
}

void Tameran::GameWindow::Shutdown()
{
	IGameWindow::Shutdown();
	m_ready = false;
}

bool Tameran::GameWindow::Update(float eTime)
{
	return true;
}

bool Tameran::GameWindow::Draw(float eTime)
{
	bool result = true;

	//Clear the buffers to begin the scene
	m_direct3D.BeginFrame(DirectX::Colors::CornflowerBlue);

	//Draw something

	//Present the rendered scene to the screen
	m_direct3D.EndFrame();

	return true;
}
