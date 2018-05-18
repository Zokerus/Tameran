#include "GameScreen.h"

Tameran::GameScreen::GameScreen(GameWindow * _game, Hydro::Direct3D * _direct3D, Hydro::ShaderManager * _manager, Hydro::Camera * _camera, Hydro::Input * _input)
	: IGameState(_direct3D, _manager, _camera), gameRef(_game), input(_input), controlManager()
{
}

Tameran::GameScreen::~GameScreen()
{
	//Reset the input pointer
	input = nullptr;

	//Reset the gameRef pointer
	gameRef = nullptr;
}

void Tameran::GameScreen::LoadContent()
{
}

bool Tameran::GameScreen::Update(float eTime)
{
	if (!controlManager.Update(eTime, input))
	{
		return false;
	}
	return true;
}

bool Tameran::GameScreen::Draw(float eTime)
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
