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
	: IGameWindow(hInst, pArgs, title, screenWidth, screenHeight, fullscreen, vsync),
	m_input(screenWidth, screenHeight), m_shaderManager()
{}

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
	if (!result)
	{
		return false;
	}

	//Initialize the input class
	result = m_input.Initialize(m_hInst, m_hWnd);
	if (!result)
	{
		ShowMessageBox("Input Error", "The input class could not be initialized");
		return false;
	}

	//Initialize the shader manager
	result = m_shaderManager.Initialize(m_direct3D.GetDevice(), m_hWnd);
	if (!result)
	{
		ShowMessageBox("ShaderManager Error", "The shader management class could not be initialized");
		return false;
	}

	m_ready = result;
	return m_ready;
}

void Tameran::GameWindow::Shutdown()
{
	//Shutdown the shader manager
	m_shaderManager.Shutdown();

	//Shutdown the input class
	m_input.Shutdown();

	IGameWindow::Shutdown();
	m_ready = false;
}

bool Tameran::GameWindow::Update(float eTime)
{
	bool result = false;
	
	//Update the input class
	result = m_input.Update();
	if (!result)
	{
		return false;
	}

	if (m_input.IsKeyDown(DIK_LALT) && m_input.IsKeyReleased(DIK_SPACE))
	{
		SwitchFullscreen(); //Just a Prototype
	}

	if (m_input.IsKeyReleased(DIK_ESCAPE))
	{
		return false;
	}

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
