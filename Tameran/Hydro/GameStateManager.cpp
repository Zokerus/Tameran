#include "GameStateManager.h"

Hydro::GameStateManager::GameStateManager()
	: gameState()
{
}

Hydro::GameStateManager::~GameStateManager()
{
	//Remove all states which are still listed
	while (gameState.size() > 0)
	{
		RemoveState();
	}
	gameState.clear();
}

bool Hydro::GameStateManager::Update(float eTime)
{
	//Update all game states which are activated
	for (int i = 0; i < gameState.size(); i++)
	{
		if (gameState[i]->IsEnable())
		{
			if (!gameState[i]->Update(eTime))
			{
				return false;
			}
		}
	}

	return true;
}

bool Hydro::GameStateManager::Draw(float eTime)
{
	//Draw all game states which are visible
	for (int i = 0; i < gameState.size(); i++)
	{
		if (gameState[i]->IsVisible())
		{
			if (!gameState[i]->Draw(eTime))
			{
				return false;
			}
		}
	}
	return true;
}

int Hydro::GameStateManager::GetGameStateCount() const
{
	//Return the number of listed game states
	return static_cast<int>(gameState.size());
}

Hydro::IGameState* Hydro::GameStateManager::CurrentState() const
{
	//Return the current working game state
	return gameState.back();
}

void Hydro::GameStateManager::AddState(IGameState *state)
{
	//Add a new game state and change the focus
	gameState.push_back(state);
	OnStateChange();
}

void Hydro::GameStateManager::ChangeState(IGameState *state)
{
	//Remove all listed game states
	while (gameState.size() > 0)
	{
		RemoveState();
	}

	//Add the new game state
	AddState(state);
}

void Hydro::GameStateManager::RemoveState()
{
	//Remove the last added game state
	if (gameState.size() > 0)
	{
		gameState.pop_back();
		OnStateChange();
	}
}

void Hydro::GameStateManager::OnStateChange()
{
	//Set the attributes of all states
	for (int i = 0; i < gameState.size(); i++)
	{
		gameState[i]->StateChange(gameState.back());
	}
}
