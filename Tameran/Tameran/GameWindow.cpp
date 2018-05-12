#include "GameWindow.h"

Tameran::GameWindow::GameWindow(HINSTANCE hInst, char* pArgs)
	: GameWindow(hInst, pArgs, "DirectX Window", 1024, 768, false, false)
{}

Tameran::GameWindow::GameWindow(HINSTANCE hInst, char* pArgs, const std::string title)
	: GameWindow(hInst, pArgs, title, 1024, 768, false, false)
{}

Tameran::GameWindow::GameWindow(HINSTANCE hInst, char* pArgs, const unsigned int screenWidth, const unsigned int screenHeight)
	: GameWindow(hInst, pArgs, "DirectX Window", screenWidth, screenHeight, false, false)
{}

Tameran::GameWindow::GameWindow(HINSTANCE hInst, char* pArgs, const std::string title, const unsigned int screenWidth, const unsigned int screenHeight)
	: GameWindow(hInst, pArgs, title, screenWidth, screenHeight, false, false)
{}

Tameran::GameWindow::GameWindow(HINSTANCE hInst, char * pArgs, const std::string title, const unsigned int screenWidth, const unsigned int screenHeight, bool fullscreen, bool vsync)
	: IGameWindow(hInst, pArgs, title, screenWidth, screenHeight, fullscreen, vsync),
	direct3D(this), shaderManager(direct3D.GetDevice(), hWnd), camera(), input(screenWidth, screenHeight, hInst, hWnd), gameStateManager(), titleScreen(this, &direct3D, &shaderManager, &camera, &input),
	menuScreen(this, &direct3D, &shaderManager, &camera, &input)
{
	//Set the initial position of the camera and build the matrices needed for rendering
	camera.SetPosition(DirectX::XMFLOAT3(0.0f, 0.0f, -10.0f));
	camera.Render(true);

	//add the title screen to the gamestate manager
	gameStateManager.ChangeState(&titleScreen);
}

Tameran::GameWindow::~GameWindow()
{}

bool Tameran::GameWindow::Update(float eTime)
{
	bool result = true;
	
	//Update the input class
	result = input.Update();
	if (!result)
	{
		return false;
	}

	//Update the gamestate manager
	result = gameStateManager.Update(eTime);
	if (!result)
	{
		return false;
	}

	if (input.IsKeyDown(DIK_LALT) && input.IsKeyReleased(DIK_SPACE))
	{
		SwitchFullscreen(); //Just a Prototype
		direct3D.SetFullScreen(fullscreen);
	}

	if (input.IsKeyReleased(DIK_ESCAPE))
	{
		return false;
	}

	return true;
}

bool Tameran::GameWindow::Draw(float eTime)
{
	bool result = true;

	//Clear the buffers to begin the scene
	direct3D.BeginFrame(DirectX::Colors::CornflowerBlue);

	//Draw something
	//Render the gamestate manager
	result = gameStateManager.Draw(eTime);
	if (!result)
	{
		return false;
	}

	//Present the rendered scene to the screen
	direct3D.EndFrame();

	return true;
}
