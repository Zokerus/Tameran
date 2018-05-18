#include "LoadingScreen.h"
#include "../GameWindow.h"
#include <Graphics/2D/Forms/Rectangle.h>

using namespace Hydro;

Tameran::LoadingScreen::LoadingScreen(GameWindow * _game, Hydro::Direct3D * _direct3D, Hydro::ShaderManager * _manager, Hydro::Camera * _camera, Hydro::Input * _input)
	: IGameState(_direct3D, _manager, _camera), gameRef(_game), input(_input), controlManager(), background(_direct3D, "Background", Hydro::Rectangle(0, 0, gameRef->GetWidth(), gameRef->GetHeight()), "Data/Images/titlescreen32.tga", gameRef->GetWidth(), gameRef->GetHeight()),
	bar(_direct3D, "ProgressBar", Hydro::Rectangle( 30,gameRef->GetHeight() - 70,10,50), "Data/Images/ProgressBar.tga", gameRef->GetWidth(), gameRef->GetHeight(), true)
{
	//Set the picture box as not selectable
	background.SetTabStop(false);

	//Add background picture box to the control manager
	controlManager.AddControl(&background);
	controlManager.AddControl(&bar);
}

Tameran::LoadingScreen::~LoadingScreen()
{
	//Reset the input pointer
	input = nullptr;

	//Reset the gameRef pointer
	gameRef = nullptr;
}

bool Tameran::LoadingScreen::Update(float eTime)
{
	if (!controlManager.Update(eTime, input))
	{
		return false;
	}

	waitingTime -= eTime;
	if (waitingTime < 0.0f)
	{
		gameRef->gameStateManager.ChangeState(&(gameRef->gameScreen));
	}

	return true;
}

bool Tameran::LoadingScreen::Draw(float eTime)
{
	bool result;
	DirectX::XMMATRIX world, view, projection, baseView, ortho;

	//Generate the view matrix based on the camera`s position
	camera->Render(false);

	//Get the all the matrices that are needed for rendering
	direct3D->GetWorldMatrix(world);
	direct3D->GetOrthoMatrix(ortho);
	direct3D->GetProjectionMatrix(projection);
	camera->GetViewMatrix(view);
	camera->GetBaseViewMatrix(baseView);

	//Turn off the Z buffer to begin 2D drawing
	direct3D->TurnZBufferOff();
	direct3D->TurnAlphaBlendingOn();

	//Draw the control objects
	result = controlManager.Draw(eTime, direct3D->GetDeviceContext(), shaderManager, world, view, ortho);
	if (!result)
	{
		return false;
	}
	//Turn Z buffer back on, for other rendering stuff
	direct3D->TurnZBufferOn();
	direct3D->TurnAlphaBlendingOff();

	return true;
}

void Tameran::LoadingScreen::SetContent()
{
	//Pass everything that is needed
	gameRef->gameScreen.LoadContent();
}
