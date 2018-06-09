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
		ProgressBar(Direct3D* _direct3D, std::string _name, Hydro::Rectangle _rect, std::string _textureFileName, int _screenWidth, int _screenHeight);

		bool Update(float eTime);
		bool Draw(float eTime, ID3D11DeviceContext *deviceContext, ShaderManager *shaderManager, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX orthoMatrix);

		void SetPosition(ID3D11DeviceContext * deviceContext, DirectX::XMINT2 _pos);

		void SetValue(short _value);
		void IncreaseValue(short add);
		void DecreaseValue(short dec);

		void SetMaxValue(short _maxValue);
		short GetMaxValue() const;

	private:
		Rectangle rect;
		Rectangle inner;
		Sprite border;
		//Sprite borderEnd;
		//Sprite borderMid;
		Sprite filling;
		//Sprite fillingEnd;
		//Sprite fillingMid;
		short value = 0;
		short maxValue = 100;
	};
}

#endif // !PROGRESSBAR

