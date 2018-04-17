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
	m_direct3D(this), m_shaderManager(m_direct3D.GetDevice(), m_hWnd), m_camera(), m_input(screenWidth, screenHeight), m_gameStateManager(), m_titleScreen(this, &m_direct3D, &m_shaderManager, &m_camera, &m_input)
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
	bool result = true;

	//Set the initial position of the camera and build the matrices needed for rendering
	m_camera.SetPosition(DirectX::XMFLOAT3(0.0f, 0.0f, -10.0f));
	m_camera.Render(true);

	//Initialize the input class
	result = m_input.Initialize(m_hInst, m_hWnd);
	if (!result)
	{
		ShowMessageBox("Input Error", "The input class could not be initialized");
		return false;
	}

	//Initialize the title screen
	result = m_titleScreen.Initialize();
	if (!result)
	{
		ShowMessageBox("Input Error", "The titlescreen could not be initialized");
		return false;
	}

	//add the title screen to the gamestate manager
	m_gameStateManager.ChangeState(&m_titleScreen);

	m_ready = result;
	return m_ready;
}

void Tameran::GameWindow::Shutdown()
{
	m_gameStateManager.~GameStateManager();

	m_titleScreen.Shutdown();
	m_titleScreen.~TitleScreen();


	//Shutdown the input class
	m_input.Shutdown();

	m_ready = false;
}

bool Tameran::GameWindow::Update(float eTime)
{
	bool result = true;
	
	//Update the input class
	result = m_input.Update();
	if (!result)
	{
		return false;
	}

	//Update the gamestate manager
	result = m_gameStateManager.Update(eTime);
	if (!result)
	{
		return false;
	}

	if (m_input.IsKeyDown(DIK_LALT) && m_input.IsKeyReleased(DIK_SPACE))
	{
		SwitchFullscreen(); //Just a Prototype
		m_direct3D.SetFullScreen(m_fullscreen);
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
	//Render the gamestate manager
	result = m_gameStateManager.Draw(eTime);
	if (!result)
	{
		return false;
	}

	//Present the rendered scene to the screen
	m_direct3D.EndFrame();

	return true;
}
