#include "TitleScreen.h"
#include "../GameWindow.h"

using namespace Hydro;

Tameran::TitleScreen::TitleScreen(GameWindow* _game, Direct3D* _direct3D, ShaderManager* _manager, Camera* _camera, Input* _input)
	: IGameState(_direct3D, _manager, _camera), gameRef(_game), input(_input), controlManager(), background(_direct3D, "Background", Hydro::Rectangle(0,0, gameRef->GetWidth(), gameRef->GetHeight()), "Data/Images/titlescreen32.tga", gameRef->GetWidth(), gameRef->GetHeight()),
	 font(direct3D->GetDevice(), direct3D->GetDeviceContext(), "Data/Font/font01", 18.0f, 3), 
 	label(direct3D, &font, gameRef->GetWidth(), gameRef->GetHeight(), 30, "Start_Label", "Press ENTER to start", DirectX::XMINT2(0,0), DirectX::XMFLOAT2(0,0),DirectX::Colors::White, DirectX::Colors::Yellow, std::bind(&TitleScreen::StartLabel_Selected, this, std::placeholders::_1))
{
	//Set the picture box as not selectable
	background.SetTabStop(false);

	//Measure the width of the label text
	float size = font.GetTextWidth(label.GetString());

	//Calculate the screen center
	float xPos = (gameRef->GetWidth() - size) / 2;

	//Set initial position and size of the start label
	label.SetPosition(DirectX::XMINT2((int)xPos, 368));
	label.SetSize(DirectX::XMFLOAT2(font.GetFontHeight(), size));

	//Add background picture box to the control manager
	controlManager.AddControl(&background);
	controlManager.AddControl(&label);

	//set focus on startLabel
	label.SetFocus(true);
}

Tameran::TitleScreen::~TitleScreen()
{
	//Reset the input pointer
	input = nullptr;

	//Reset the gameRef pointer
	gameRef = nullptr;
}

bool Tameran::TitleScreen::Update(float eTime)
{
	if (!controlManager.Update(eTime, input))
	{
		return false;
	}

	return true;
}

bool Tameran::TitleScreen::Draw(float eTime)
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

void Tameran::TitleScreen::StartLabel_Selected(const Hydro::IControl *sender)
{
	gameRef->gameStateManager.ChangeState(&(gameRef->menuScreen));
}
