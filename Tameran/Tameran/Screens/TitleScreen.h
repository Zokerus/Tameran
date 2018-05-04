#ifndef TITLESCREEN
#define TITLESCREEN

//My Includes
#include <IGameState.h>
#include <GameStateManager.h>
#include <Input/Input.h>
#include <Controls/ControlManager.h>
#include <Controls/PictureBox.h>
#include <Controls/LinkLabel.h>
#include <Graphics/Text/Font.h>

//////////////////////////////////////////////////////////////////////////
//Class: TitleScreen
//////////////////////////////////////////////////////////////////////////
namespace Tameran
{
	class TitleScreen : public Hydro::IGameState
	{
	public:
		TitleScreen(class GameWindow* _game, Hydro::Direct3D* _direct3D, Hydro::ShaderManager* _manager, Hydro::Camera* _camera, Hydro::Input* _input);
		~TitleScreen();

		bool Update(float eTime);
		bool Draw(float eTime);

	private:
		void StartLabel_Selected(const Hydro::IControl* sender);

	private:
		Tameran::GameWindow* gameRef;
		Hydro::Input* input;
		Hydro::ControlManager controlManager;
		Hydro::PictureBox background;
		Hydro::Font font;
		Hydro::LinkLabel label;
	};
}

#endif // !TITLESCREEN