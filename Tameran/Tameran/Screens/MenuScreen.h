#ifndef MENUSCREEN
#define MENUSCREEN

//My Includes
#include <IGameState.h>
#include <GameStateManager.h>
#include <Input/Input.h>
#include <Controls/ControlManager.h>
#include <Controls/PictureBox.h>
#include <Controls/LinkLabel.h>
#include <Graphics/Text/Font.h>

//////////////////////////////////////////////////////////////////////////
//Class: MenuScreen
//////////////////////////////////////////////////////////////////////////
namespace Tameran
{
	class MenuScreen : public Hydro::IGameState
	{
	public:
		MenuScreen(class GameWindow* _game, Hydro::Direct3D* _direct3D, Hydro::ShaderManager* _manager, Hydro::Camera* _camera, Hydro::Input* _input);
		~MenuScreen();

		bool Update(float eTime);
		bool Draw(float eTime);

	private:
		void MenuItem_Selected(Hydro::IControl* sender);
		void MenuItem_Changed(const Hydro::IControl* sender);
		void ArrangeMenuItems(Hydro::LinkLabel& label, int& yPos);
		
	private:
		Tameran::GameWindow* gameRef;
		Hydro::Input* input;
		Hydro::ControlManager controlManager;
		Hydro::PictureBox background;
		Hydro::Font font;

		Hydro::LinkLabel continueGame;
		Hydro::LinkLabel startGame;
		Hydro::LinkLabel loadGame;
		Hydro::LinkLabel options;
		Hydro::LinkLabel exitGame;

	};
}
#endif // !MENUSCREEN

