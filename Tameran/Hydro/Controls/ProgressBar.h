#ifndef PROGRESSBAR
#define PROGRESSBAR

//My Includes
#include "IControl.h"
#include "../Graphics/Direct3D.h"
#include "../Input/Input.h"
#include "../Graphics/2D/Sprite.h"
#include "../Graphics/2D/Forms/Rectangle.h"

//////////////////////////////////////////////////////////////////////////
//Class: ProgressBar
//////////////////////////////////////////////////////////////////////////
namespace Hydro
{
	class ProgressBar : public IControl
	{
	public:
		ProgressBar(Direct3D* _direct3D, std::string _name, Hydro::Rectangle _rect, std::string _textureFileName, int _screenWidth, int _screenHeight, bool _border);

		bool Update(float eTime);
		bool Draw(float eTime, ID3D11DeviceContext *deviceContext, ShaderManager *shaderManager, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX orthoMatrix);

		void SetPosition(ID3D11DeviceContext * deviceContext, DirectX::XMINT2 pos);

	private:
		bool border;
		Sprite borderBeg;
		Sprite borderEnd;
		//Sprite borderMid;
		//Sprite fillingBeg;
		//Sprite fillingEnd;
		//Sprite fillingMid;
	};
}

#endif // !PROGRESSBAR
