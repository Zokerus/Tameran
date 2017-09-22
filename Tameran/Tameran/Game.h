#ifndef GAMEHEADER
#define GAMEHEADER

//My Includes
#include <IGame.h>

//////////////////////////////////////////////////////////////////////////
//Class: Game
//////////////////////////////////////////////////////////////////////////
namespace Tameran
{
	class Game : public Hydro::IGame
	{
	public:
		Game();
		Game(const Game& other) = delete;
		Game& operator=(Game& other) = delete;
		~Game();

		bool Initialize(HWND hWnd, HINSTANCE hInst, unsigned int screenWidth, unsigned int screenHeight);
		void Shutdown();

	private:
		bool Update(float eTime);
		bool Draw(float eTime);

	private:
		char* m_workingDirectory;
	};
}

#endif // !GAMEHEADER

