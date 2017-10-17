#ifndef IGAMESTATE
#define IGAMESTATE

//My Includes
#include "Graphics/Direct3D.h"

//////////////////////////////////////////////////////////////////////////
//Class: IGameState
//////////////////////////////////////////////////////////////////////////
namespace Hydro
{
	class IGameState
	{
	public:
		IGameState(Direct3D *direct3D);
		~IGameState();

		bool Initialize();
		void Shutdown();

		virtual bool Update(float eTime);
		virtual bool Draw(float eTime);

		bool IsVisible() const;
		bool IsEnable() const;

		void SetVisibility(const bool state);
		void Enable(const bool state);
		void StateChange(IGameState *currState);

	protected:
		Direct3D *m_direct3D;
		bool m_ready, m_visible, m_enable;
	};
}

#endif // !IGAMESTATE

