#include "TitleScreen.h"
#include "../GameWindow.h"

using namespace Hydro;

Tameran::TitleScreen::TitleScreen(GameWindow* game, Direct3D* direct3D, ShaderManager* manager, Camera* camera, Input* input)
	: IGameState(direct3D, manager, camera), m_gameRef(game), m_input(input), m_background()
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

	//The Screen is ready to draw and use
	m_ready = true;

	return true;
}

void Tameran::TitleScreen::Shutdown()
{
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
	result = m_shaderManager->RenderTextureShader(m_direct3D->GetDeviceContext(), m_background.GetIndexCount(), world, view, ortho, m_background.GetTexture());
	if (!result)
		return false;

	//Turn Z buffer back on, for other rendering stuff
	m_direct3D->TurnZBufferOn();
	m_direct3D->TurnAlphaBlendingOff();

	return true;
}
