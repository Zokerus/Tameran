#ifndef TITLESCREEN
#define TITLESCREEN

//My Includes
#include <IGameState.h>
#include <GameStateManager.h>
#include <Input/Input.h>
#include <Controls/ControlManager.h>
#include <Controls\PictureBox.h>
#include <Graphics/Text/Font.h>
#include <Graphics/Text/Text.h>

//////////////////////////////////////////////////////////////////////////
//Class: TitleScreen
//////////////////////////////////////////////////////////////////////////
namespace Tameran
{
	class TitleScreen : public Hydro::IGameState
	{
	public:
		TitleScreen(class GameWindow* game, Hydro::Direct3D* direct3D, Hydro::ShaderManager* manager, Hydro::Camera* camera, Hydro::Input* input);
		~TitleScreen();

		bool Initialize();
		void Shutdown();

		bool Update(float eTime);
		bool Draw(float eTime);

	private:
		Tameran::GameWindow* m_gameRef;
		Hydro::Input* m_input;
		Hydro::ControlManager m_controlManager;
		Hydro::PictureBox m_background;
		Hydro::Font m_font;
		Hydro::Text m_text;
	};
}

#endif // !TITLESCREEN

