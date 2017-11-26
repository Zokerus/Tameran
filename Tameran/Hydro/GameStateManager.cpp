#include "GameStateManager.h"

Hydro::GameStateManager::GameStateManager()
	: m_gameState()
{
}

Hydro::GameStateManager::~GameStateManager()
{
	//Remove all states which are still listed
	while (m_gameState.size() > 0)
	{
		RemoveState();
	}
	m_gameState.clear();
	m_gameState.~vector();
}

bool Hydro::GameStateManager::Update(float eTime)
{
	//Update all game states which are activated
	for (int i = 0; i < m_gameState.size(); i++)
	{
		if (m_gameState[i]->IsEnable())
		{
			if (!m_gameState[i]->Update(eTime))
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
	for (int i = 0; i < m_gameState.size(); i++)
	{
		if (m_gameState[i]->IsVisible())
		{
			if (!m_gameState[i]->Draw(eTime))
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
	return static_cast<int>(m_gameState.size());
}

Hydro::IGameState* Hydro::GameStateManager::CurrentState() const
{
	//Return the current working game state
	return m_gameState.back();
}

void Hydro::GameStateManager::AddState(IGameState *state)
{
	//Add a new game state and change the focus
	m_gameState.push_back(state);
	OnStateChange();
}

void Hydro::GameStateManager::ChangeState(IGameState *state)
{
	//Remove all listed game states
	while (m_gameState.size() > 0)
	{
		RemoveState();
	}

	//Add the new game state
	AddState(state);
}

void Hydro::GameStateManager::RemoveState()
{
	//Remove the last added game state
	if (m_gameState.size() > 0)
	{
		m_gameState.pop_back();
		OnStateChange();
	}
}

void Hydro::GameStateManager::OnStateChange()
{
	//Set the attributes of all states
	for (int i = 0; i < m_gameState.size(); i++)
	{
		m_gameState[i]->StateChange(m_gameState.back());
	}
}
