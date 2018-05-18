#include "ProgressBar.h"

Hydro::ProgressBar::ProgressBar(Direct3D* _direct3D, std::string _name, Hydro::Rectangle _rect, std::string _textureFileName, int _screenWidth, int _screenHeight, bool _border)
 : IControl(_name, DirectX::XMFLOAT2((float)_rect.GetWidth(), (float)_rect.GetHeight()), DirectX::XMINT2(_rect.GetXPos(), _rect.GetYPos()), false, true, true, false, DirectX::Colors::Black, ControlType::ProgressBar),
	borderBeg(_direct3D->GetDevice(), _direct3D->GetDeviceContext(), _screenWidth, _screenHeight, _textureFileName, Rectangle(0,0,10,50)),
	borderEnd(_direct3D->GetDevice(), _direct3D->GetDeviceContext(), _screenWidth, _screenHeight, _textureFileName, Rectangle(190, 0, 10, 50))
{
	Rectangle temp;
	if (_rect.GetWidth < 20)
	{
		temp.SetParameters(_rect.GetXPos(), _rect.GetYPos(), _rect.GetWidth() / 2, _rect.GetHeight());
	}
	else
	{
		temp.SetParameters(_rect.GetXPos(), _rect.GetYPos(), 10, _rect.GetHeight());
	}


	//Set the initial position of the pictureBox
	if (!borderBeg.Update(_direct3D->GetDeviceContext(), Rectangle(_rect.GetXPos(), _rect.GetYPos(), _rect.GetWidth()))
	{
		throw std::exception(("Failed to position and create a sprite: " + _textureFileName).c_str());
	}

	//Set the initial position of the pictureBox
	if (!borderBeg.Update(_direct3D->GetDeviceContext(), pos))
	{
		throw std::exception(("Failed to position and create a sprite: " + _textureFileName).c_str());
	}
}

bool Hydro::ProgressBar::Update(float eTime)
{
	return true;
}

bool Hydro::ProgressBar::Draw(float eTime, ID3D11DeviceContext* deviceContext, ShaderManager* shaderManager, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX orthoMatrix)
{
	bool result;

	//Prepare the picture box sprite for drawing
	result = borderBeg.Render(deviceContext);
	if (!result)
	{
		return false;
	}

	//Render the sprite object with the texture shader of the shader manager
	result = shaderManager->RenderTextureShader(deviceContext, borderBeg.GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, borderBeg.GetTexture());
	if (!result)
	{
		return false;
	}

	return true;
}

void Hydro::ProgressBar::SetPosition(ID3D11DeviceContext * deviceContext, DirectX::XMINT2 pos)
{
	pos = pos;

	//Update the position of the sprite
	borderBeg.Update(deviceContext, pos);
}
