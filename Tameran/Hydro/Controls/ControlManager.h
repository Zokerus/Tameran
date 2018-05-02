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

		bool Update(float eTime, Input* input);
		bool Draw(float eTime, ID3D11DeviceContext* deviceContext, ShaderManager* shaderManager, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX orthoMatrix);

		int ControlCount();

		void AddControl(IControl* Ccontrol);
		void RemoveControl();

		void NextControl();
		void PrevControl();
		void SetFocusChangeEvent(std::function<void(const IControl*)> func);

	private:
		std::vector<IControl*> control;
		int selectedControl;

		std::function<void(const IControl*)> focusChanged;

	};
}

#endif // !CONTROLMANAGER
