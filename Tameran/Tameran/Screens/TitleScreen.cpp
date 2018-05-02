#include "TitleScreen.h"
#include "../GameWindow.h"

using namespace Hydro;

Tameran::TitleScreen::TitleScreen(GameWindow* game, Direct3D* direct3D, ShaderManager* manager, Camera* camera, Input* input)
	: IGameState(direct3D, manager, camera), m_gameRef(game), m_input(input), m_controlManager(), m_background(direct3D, "Background", Hydro::Rectangle(0,0, m_gameRef->GetWidth(), m_gameRef->GetHeight()), "Data/Images/titlescreen32.tga", m_gameRef->GetWidth(), m_gameRef->GetHeight()),
	 m_font(m_direct3D->GetDevice(), m_direct3D->GetDeviceContext(), "Data/Font/font01", 18.0f, 3), 
 	m_label(direct3D, &m_font, game->GetWidth(), game->GetHeight(), 30, "Start_Label", "Press ENTER to start", DirectX::XMINT2(0,0), DirectX::XMFLOAT2(0,0),DirectX::Colors::White, DirectX::Colors::Yellow, std::bind(&TitleScreen::StartLabel_Selected, this, std::placeholders::_1))
{
	//Measure the width of the label text
	float size = m_font.GetTextWidth(m_label.GetString());

	//Calculate the screen center
	float xPos = (m_gameRef->GetWidth() - size) / 2;

	//Set initial position and size of the start label
	m_label.SetPosition(DirectX::XMINT2((int)xPos, 368));
	m_label.SetSize(DirectX::XMFLOAT2(m_font.GetFontHeight(), size));

	//Add background picture box to the control manager
	m_controlManager.AddControl(&m_background);
	m_controlManager.AddControl(&m_label);

	//set focus on startLabel
	m_label.SetFocus(true);
}

Tameran::TitleScreen::~TitleScreen()
{
	if (m_ready)
	{
		Shutdown();
	}
}

bool Tameran::TitleScreen::Initialize()
{
	bool result = true;

	//Initialize the parent class
	IGameState::Initialize();

	//Create screen rectangle
	Hydro::Rectangle rect(0, 0, m_gameRef->GetWidth(), m_gameRef->GetHeight());

	//Initialize the background image object
	//result = m_background.Initialize(m_direct3D, rect, "Data/Images/titlescreen32.tga", m_gameRef->GetWidth(), m_gameRef->GetHeight());
	if (!result)
	{
		return false;
	}

	//Set the picture box as not selectable
	m_background.SetTabStop(false);
	
	//Measure the width of the label text
	//float size = m_font.GetTextWidth("Press ENTER to start");

	//Calculate the screen center
	//float xPos = (m_gameRef->GetWidth() - size) / 2;

	//test of text
	//result = m_label.Initialize(m_direct3D, &m_font, "Press ENTER to start", DirectX::XMINT2((int)xPos, 368), DirectX::XMFLOAT2(m_font.GetFontHeight(), size), DirectX::Colors::White);
	if (!result)
	{
		OutputDebugString("Text could not be initialized!");
		return false;
	}

	//Add background picture box to the control manager
	//m_controlManager.AddControl(&m_background);
	//m_controlManager.AddControl(&m_label);

	//set focus on startLabel
	m_label.SetFocus(true);

	//The Screen is ready to draw and use
	m_ready = true;

	return true;
}

void Tameran::TitleScreen::Shutdown()
{
	//Shutdown text object
	m_label.Shutdown();

	//Shutdown the font object
	//m_font.Shutdown();

	//Shutdown the background 
	m_background.Shutdown();

	//Reset the input pointer
	m_input = nullptr;

	//Reset the gameRef pointer
	m_gameRef = nullptr;

	//Shutdown the parent class
	IGameState::Shutdown();
}

bool Tameran::TitleScreen::Update(float eTime)
{
	//do not update if clss is not ready
	if (!m_ready)
	{
		return false;
	}

	if (!m_controlManager.Update(eTime, m_input))
	{
		return false;
	}

	return true;
}

bool Tameran::TitleScreen::Draw(float eTime)
{
	bool result;
	DirectX::XMMATRIX world, view, projection, baseView, ortho;

	//Do not draw the class if it is not ready
	if (!m_ready)
	{
		return false;
	}

	//Generate the view matrix based on the camera`s position
	m_camera->Render(false);

	//Get the all the matrices that are needed for rendering
	m_direct3D->GetWorldMatrix(world);
	m_direct3D->GetOrthoMatrix(ortho);
	m_direct3D->GetProjectionMatrix(projection);
	m_camera->GetViewMatrix(view);
	m_camera->GetBaseViewMatrix(baseView);

	//Turn off the Z buffer to begin 2D drawing
	m_direct3D->TurnZBufferOff();
	m_direct3D->TurnAlphaBlendingOn();

	//Draw the control objects
	result = m_controlManager.Draw(eTime, m_direct3D->GetDeviceContext(), m_shaderManager, world, view, ortho);
	if (!result)
	{
		return false;
	}
	//Turn Z buffer back on, for other rendering stuff
	m_direct3D->TurnZBufferOn();
	m_direct3D->TurnAlphaBlendingOff();

	return true;
}

void Tameran::TitleScreen::StartLabel_Selected(const Hydro::IControl *sender)
{
	m_gameRef->Kill();
}
