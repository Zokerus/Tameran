#ifndef GAMESCREEN
#define GAMESCREEN

//My Includes
#include <IGameState.h>
#include <GameStateManager.h>
#include <Input/Input.h>
#include <Controls/ControlManager.h>

#include <Graphics/Models/Model.h>

//////////////////////////////////////////////////////////////////////////
//Class: GameScreen
//////////////////////////////////////////////////////////////////////////
namespace Tameran
{
	class GameScreen : public Hydro::IGameState
	{
	public:
		GameScreen(class GameWindow* _game, Hydro::Direct3D* _direct3D, Hydro::ShaderManager* _manager, Hydro::Camera* _camera, Hydro::Input* _input);
		~GameScreen();

		void LoadContent();

		bool Update(float eTime);
		bool Draw(float eTime);

	private:
		Tameran::GameWindow* gameRef;
		Hydro::Input* input;
		Hydro::ControlManager controlManager;

		Hydro::Model test;
	};
}

#endif // !GAMESCREEN
