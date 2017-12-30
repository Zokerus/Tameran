#ifndef CONTROLMANAGER
#define CONTROLMANAGER

//Includes
#include <vector>
#include <functional>

//My Includes
#include "IControl.h"
#include "../Graphics/Shader/ShaderManager.h"
#include "../Input/Input.h"

//////////////////////////////////////////////////////////////////////////
//Class: ControlManager
//////////////////////////////////////////////////////////////////////////
namespace Hydro
{
	class ControlManager
	{
	public:
		ControlManager();
		~ControlManager();

		//bool Initialize(); not needed as this point
		void Shutdown();

		bool Update(float eTime, Input *input);
		bool Draw(float eTime, ID3D11DeviceContext *deviceContext, ShaderManager *shaderManager, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX orthoMatrix);

		int ControlCount();

		void AddControl(IControl *control);
		void RemoveControl();

		void NextControl();
		void PrevControl();
		void SetFocusChangeEvent(std::function<void(const IControl*)> func);

	private:
		std::vector<IControl*> m_control;
		int m_selectedControl;

		std::function<void(const IControl*)> m_focusChanged;

	};
}

#endif // !CONTROLMANAGER
