#ifndef LOADINGSCREEN
#define LOADINGSCREEN

//My Includes
#include <IGameState.h>
#include <GameStateManager.h>
#include <Input/Input.h>
#include <Controls/ControlManager.h>
#include <Controls/PictureBox.h>
#include <Controls/ProgressBar.h>

//////////////////////////////////////////////////////////////////////////
//Class: LoadingScreen
//////////////////////////////////////////////////////////////////////////
namespace Tameran
{
	class LoadingScreen : public Hydro::IGameState
	{
	public:
		LoadingScreen(class GameWindow* _game, Hydro::Direct3D* _direct3D, Hydro::ShaderManager* _manager, Hydro::Camera* _camera, Hydro::Input* _input);
		~LoadingScreen();

		bool Update(float eTime);
		bool Draw(float eTime);

		void SetContent();

	private:
		Tameran::GameWindow* gameRef;
		Hydro::Input* input;
		Hydro::ControlManager controlManager;
		Hydro::PictureBox background;
		Hydro::ProgressBar bar;
		float waitingTime = 1.0f;
	};
}
#endif // !LOADINGSCREEN

