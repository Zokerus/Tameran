#include "Game.h"

Tameran::Game::Game()
	: IGame(), m_workingDirectory(nullptr)
{}

Tameran::Game::~Game()
{
	if (m_ready)
	{
		Shutdown();
	}
}

bool Tameran::Game::Initialize(HWND hWnd, HINSTANCE hInst, unsigned int screenWidth, unsigned int screenHeight)
{
	bool result = false;

	//Call the initialize method of the parent object
	result = IGame::Initialize(hWnd, hInst, screenWidth, screenHeight);
	if (!result)
	{
		return false;
	}

	//Set m_ready to true to flag the game object as initialized
	m_ready = true;
	return true;
}

void Tameran::Game::Shutdown()
{
	m_ready = false;
	IGame::Shutdown();
}

bool Tameran::Game::Update(float eTime)
{
	bool result = false;

	//Do some processing and updating stuff

	return true;
}

bool Tameran::Game::Draw(float eTime)
{
	bool result = false;

	//Render and draw something

	return true;
}
