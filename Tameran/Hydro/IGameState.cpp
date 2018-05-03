#include "IGameState.h"

Hydro::IGameState::IGameState(Direct3D* _direct3D, ShaderManager* _manager, Camera* _camera)
	: direct3D(_direct3D), shaderManager(_manager), camera(_camera), visible(true), enable(true)
{}

Hydro::IGameState::~IGameState()
{
	//Reset the camera pointer
	camera = nullptr;

	//Reset the shader manager pointer
	shaderManager = nullptr;

	//Reset the direct3D object pointer
	direct3D = nullptr;
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
	return visible;
}

bool Hydro::IGameState::IsEnable() const
{
	//Return the activity state of the game state, will it be drawn and updated
	return enable;
}

void Hydro::IGameState::SetVisibility(const bool state)
{
	//Set the visibility state
	visible = state;
}

void Hydro::IGameState::Enable(const bool state)
{
	//Set the activity state
	enable = state;
}

void Hydro::IGameState::StateChange(IGameState *currState)
{
	if (currState == this)
	{
		enable = true;
		visible = true;
	}
	else
	{
		enable = false;
		visible = false;
	}
}
