#ifndef GAMESTATEMANAGER
#define GAMESTATEMANAGER

//Includes
#include <vector>

//My Includes
#include "IGameState.h"

//////////////////////////////////////////////////////////////////////////
//Class: GameStateManager
//////////////////////////////////////////////////////////////////////////
namespace Hydro
{
	class GameStateManager
	{
	public:
		GameStateManager();
		~GameStateManager();

		bool Update(float eTime);
		bool Draw(float eTime);

		int GetGameStateCount() const;
		IGameState* CurrentState() const;

		void AddState(IGameState *state);
		void ChangeState(IGameState *state);
		void RemoveState();

	private:
		void OnStateChange();

	private:
		std::vector<IGameState*> m_gameState;
	};
}
#endif // !GAMESTATEMANAGER

