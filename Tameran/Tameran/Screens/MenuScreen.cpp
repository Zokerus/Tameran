#include "MenuScreen.h"
#include "../GameWindow.h"

Tameran::MenuScreen::MenuScreen(GameWindow* _game, Hydro::Direct3D* _direct3D, Hydro::ShaderManager* _manager, Hydro::Camera* _camera, Hydro::Input* _input)
	:IGameState(_direct3D, _manager, _camera), gameRef(_game), input(_input), controlManager(), background(_direct3D, "Background", Hydro::Rectangle(0, 0, gameRef->GetWidth(), gameRef->GetHeight()), "Data/Images/titlescreen32.tga", gameRef->GetWidth(), gameRef->GetHeight()),
	font(direct3D->GetDevice(), direct3D->GetDeviceContext(), "Data/Font/font01", 36.0f, 3),
	continueGame(direct3D, &font, gameRef->GetWidth(), gameRef->GetHeight(), 30, "Continue", "The story continues", DirectX::XMINT2(0, 0), DirectX::XMFLOAT2(0, 0), DirectX::Colors::White, DirectX::Colors::Yellow, std::bind(&MenuScreen::MenuItem_Selected, this, std::placeholders::_1)),
	startGame(direct3D, &font, gameRef->GetWidth(), gameRef->GetHeight(), 30, "NewGame", "The story begins", DirectX::XMINT2(0, 0), DirectX::XMFLOAT2(0, 0), DirectX::Colors::White, DirectX::Colors::Yellow, std::bind(&MenuScreen::MenuItem_Selected, this, std::placeholders::_1)),
	loadGame(direct3D, &font, gameRef->GetWidth(), gameRef->GetHeight(), 30, "LoadLabel", "Load", DirectX::XMINT2(0, 0), DirectX::XMFLOAT2(0, 0), DirectX::Colors::White, DirectX::Colors::Yellow, std::bind(&MenuScreen::MenuItem_Selected, this, std::placeholders::_1)),
	options(direct3D, &font, gameRef->GetWidth(), gameRef->GetHeight(), 30, "Options", "Options", DirectX::XMINT2(0, 0), DirectX::XMFLOAT2(0, 0), DirectX::Colors::White, DirectX::Colors::Yellow, std::bind(&MenuScreen::MenuItem_Selected, this, std::placeholders::_1)),
	exitGame(direct3D, &font, gameRef->GetWidth(), gameRef->GetHeight(), 30, "Exit Game", "Exit Game", DirectX::XMINT2(0, 0), DirectX::XMFLOAT2(0, 0), DirectX::Colors::White, DirectX::Colors::Yellow, std::bind(&MenuScreen::MenuItem_Selected, this, std::placeholders::_1))
{
	int yPos = 500;

	//Set the picture box as not selectable
	background.SetTabStop(false);

	//Add all controls to the control manager
	controlManager.AddControl(&background);
	controlManager.AddControl(&continueGame);
	controlManager.AddControl(&startGame);
	controlManager.AddControl(&loadGame);
	controlManager.AddControl(&options);
	controlManager.AddControl(&exitGame);

	//Calculate positions of menu items
	ArrangeMenuItems(continueGame, yPos);
	ArrangeMenuItems(startGame, yPos);
	ArrangeMenuItems(loadGame, yPos);
	ArrangeMenuItems(options, yPos);
	ArrangeMenuItems(exitGame, yPos);

	controlManager.SetFocusChangeEvent(std::bind(&MenuScreen::MenuItem_Changed, this, std::placeholders::_1));
	controlManager.NextControl();
}

Tameran::MenuScreen::~MenuScreen()
{
	//Reset the input pointer
	input = nullptr;

	//Reset the gameRef pointer
	gameRef = nullptr;
}

bool Tameran::MenuScreen::Update(float eTime)
{
	if (!controlManager.Update(eTime, input))
	{
		return false;
	}
	return true;
}

bool Tameran::MenuScreen::Draw(float eTime)
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

	//Render view matrix
	camera->Render(false);

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

void Tameran::MenuScreen::MenuItem_Selected(Hydro::IControl* sender)
{
	if (Hydro::LinkLabel* control = dynamic_cast<Hydro::LinkLabel*>(sender))
	{
		if (control == &continueGame)
		{
			//Start the game from last saved point
			//gameRef->gameStateManager.ChangeState(
		}
		else if (control == &startGame)
		{
			//Start the game fram a new beginning
		}
		else if (control == &loadGame)
		{
			//Load an old savegame
		}
		else if (control == &options)
		{
			//Open the options menu
		}
		else if (control == &exitGame)
		{
			//exit the game
			gameRef->Exit();
		}

	}
}

void Tameran::MenuScreen::MenuItem_Changed(const Hydro::IControl* sender)
{
}

void Tameran::MenuScreen::ArrangeMenuItems(Hydro::LinkLabel& label, int& yPos)
{
	DirectX::XMFLOAT2 size = label.GetItemSize();
	DirectX::XMINT2 pos(gameRef->GetWidth() / 2 - (int)(size.x / 2.0f), yPos);
	label.SetPosition(pos);
	yPos += (int)(size.y + 5.0f);
}
