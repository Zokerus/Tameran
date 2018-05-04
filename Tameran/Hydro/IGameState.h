#ifndef IGAMESTATE
#define IGAMESTATE

//My Includes
#include "Graphics/Direct3D.h"
#include "Graphics/Shader/ShaderManager.h"
#include "Graphics\Camera.h"

//////////////////////////////////////////////////////////////////////////
//Class: IGameState
//////////////////////////////////////////////////////////////////////////
namespace Hydro
{
	class IGameState
	{
	public:
		IGameState(Direct3D* _direct3D, ShaderManager* _manager, Camera* _camera);
		~IGameState();

		virtual bool Update(float eTime);
		virtual bool Draw(float eTime);

		bool IsVisible() const;
		bool IsEnable() const;

		void SetVisibility(const bool state);
		void Enable(const bool state);
		void StateChange(IGameState* currState);

	protected:
		Direct3D* direct3D;
		ShaderManager* shaderManager;
		Camera* camera;
		bool visible, enable;
	};
}

#endif // !IGAMESTATE

