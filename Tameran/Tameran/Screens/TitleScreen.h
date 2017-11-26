#ifndef TITLESCREEN
#define TITLESCREEN

//My Includes
#include <IGameState.h>
#include <GameStateManager.h>

#include <Input/Input.h>
#include <Graphics/2D/Sprite.h>

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
		Hydro::Sprite m_background;
	};
}

#endif // !TITLESCREEN

