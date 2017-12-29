#include "TitleScreen.h"
#include "../GameWindow.h"

using namespace Hydro;

Tameran::TitleScreen::TitleScreen(GameWindow* game, Direct3D* direct3D, ShaderManager* manager, Camera* camera, Input* input)
	: IGameState(direct3D, manager, camera), m_gameRef(game), m_input(input), m_background()
	, m_font(), m_text(game->GetWidth(), game->GetHeight(), 30)
{
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
	bool result;

	//Initialize the parent class
	IGameState::Initialize();

	//Initialize the background image object
	result = m_background.Initialize(m_direct3D->GetDevice(), m_direct3D->GetDeviceContext(), m_gameRef->GetWidth() , m_gameRef->GetHeight(), "Data/Images/titlescreen32.tga", m_gameRef->GetWidth(), m_gameRef->GetHeight());
	if (!result)
	{
		return false;
	}

	m_background.Update(m_direct3D->GetDeviceContext(), 0, 0);
	
	//Initialize font object
	result = m_font.Initialize(m_direct3D->GetDevice(), m_direct3D->GetDeviceContext(), "Data/Font/font01", 18.0f, 3);
	if (!result)
	{
		OutputDebugString("Font could not be initialized!");
		return false;
	}

	//test of text
	result = m_text.Initialize(m_direct3D->GetDevice(), m_direct3D->GetDeviceContext(), false, &m_font, "Press ENTER to start", DirectX::XMINT2(20, 20), DirectX::Colors::Yellow);
	if (!result)
	{
		OutputDebugString("Text could not be initialized!");
		return false;
	}

	//The Screen is ready to draw and use
	m_ready = true;

	return true;
}

void Tameran::TitleScreen::Shutdown()
{
	//Shutdown text object
	m_text.Shutdown();

	//Shutdown the font object
	m_font.Shutdown();

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

	//Prepare background image for drawing
	result = m_background.Render(m_direct3D->GetDeviceContext());
	if (!result)
		return false;

	//Draw the background image with the texture shader
	result = m_shaderManager->RenderFontShader(m_direct3D->GetDeviceContext(), m_background.GetIndexCount(), world, view, ortho, m_background.GetTexture(), DirectX::Colors::Beige);
	if (!result)
	{
		return false;
	}

	//Draw Text
	result = m_text.Render(m_direct3D->GetDeviceContext(), m_shaderManager, world, view, ortho, m_font.GetTexture());
	if (!result)
	{
		return false;
	}

	//Turn Z buffer back on, for other rendering stuff
	m_direct3D->TurnZBufferOn();
	m_direct3D->TurnAlphaBlendingOff();

	return true;
}
