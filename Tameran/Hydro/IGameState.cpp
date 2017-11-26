#include "IGameState.h"

Hydro::IGameState::IGameState(Direct3D* direct3D, ShaderManager* manager, Camera* camera)
	: m_direct3D(direct3D), m_shaderManager(manager), m_camera(camera), m_ready(false), m_visible(true), m_enable(true)
{}

Hydro::IGameState::~IGameState()
{
	if (m_ready)
	{
		Shutdown();
	}
}

bool Hydro::IGameState::Initialize()
{
	m_ready = true;
	return true;
}

void Hydro::IGameState::Shutdown()
{
	//Reset the camera pointer
	m_camera = nullptr;

	//Reset the shader manager pointer
	m_shaderManager = nullptr;

	//Reset the direct3D object pointer
	m_direct3D = nullptr;

	m_ready = false;
}

bool Hydro::IGameState::Update(float eTime)
{
	return true;
}

bool Hydro::IGameState::Draw(float eTime)
{
	return true;
}

bool Hydro::IGameState::IsVisible() const
{
	//Return the visibility state of the game state, will it be drawn
	return m_visible;
}

bool Hydro::IGameState::IsEnable() const
{
	//Return the activity state of the game state, will it be drawn and updated
	return m_enable;
}

void Hydro::IGameState::SetVisibility(const bool state)
{
	//Set the visibility state
	m_visible = state;
}

void Hydro::IGameState::Enable(const bool state)
{
	//Set the activity state
	m_enable = state;
}

void Hydro::IGameState::StateChange(IGameState *currState)
{
	if (currState == this)
	{
		m_enable = true;
		m_visible = true;
	}
	else
	{
		m_enable = false;
		m_visible = false;
	}
}
