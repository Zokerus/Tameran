#include "ProgressBar.h"

Hydro::ProgressBar::ProgressBar(Direct3D* _direct3D, std::string _name, Hydro::Rectangle _rect, std::string _textureFileName, int _screenWidth, int _screenHeight)
 : IControl(_name, DirectX::XMFLOAT2((float)_rect.GetWidth(), (float)_rect.GetHeight()), DirectX::XMINT2(_rect.GetXPos(), _rect.GetYPos()), false, true, true, false, DirectX::Colors::Black, ControlType::ProgressBar),
	rect(_rect),inner(_rect.GetXPos(), _rect.GetYPos(), 0, _rect.GetHeight()),
	border(_direct3D->GetDevice(), _direct3D->GetDeviceContext(), _screenWidth, _screenHeight, _textureFileName, Rectangle(0,0,8,4)),
	filling(_direct3D->GetDevice(), _direct3D->GetDeviceContext(), _screenWidth, _screenHeight, _textureFileName, Rectangle(0, 4, 8, 4))
{
	//SetValue(50);	//Set the initial position of the progressBar border
	if (!border.Update(_direct3D->GetDeviceContext(), rect))
	{
		throw std::exception(("Failed to position and create a sprite: " + _textureFileName).c_str());
	}

	if (!filling.Update(_direct3D->GetDeviceContext(), inner))
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

	if (!border.Update(deviceContext, rect))
	{
		return false;
	}

	if (!filling.Update(deviceContext, inner))
	{
		return false;
	}

	//Prepare the picture box sprite for drawing
	result = border.Render(deviceContext);
	if (!result)
	{
		return false;
	}

	//Render the sprite object with the texture shader of the shader manager
	result = shaderManager->RenderTextureShader(deviceContext, border.GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, border.GetTexture());
	if (!result)
	{
		return false;
	}


	if (value > 0)
	{
		//Prepare the picture box sprite for drawing
		result = filling.Render(deviceContext);
		if (!result)
		{
			return false;
		}

		//Render the sprite object with the texture shader of the shader manager
		result = shaderManager->RenderTextureShader(deviceContext, filling.GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, filling.GetTexture());
		if (!result)
		{
			return false;
		}
	}
	return true;
}

void Hydro::ProgressBar::SetPosition(ID3D11DeviceContext* deviceContext, DirectX::XMINT2 _pos)
{
	pos = _pos;
	rect.SetParameters(pos.x, pos.y, rect.GetWidth(), rect.GetHeight());
	inner.SetParameters(pos.x, pos.y, inner.GetWidth(), rect.GetHeight());
}

void Hydro::ProgressBar::SetValue(short _value)
{
	value = _value;
	if (value > maxValue)
	{
		value = maxValue;
	}
	else if (value < 0)
	{
		value = 0;
	}
	inner.SetParameters(pos.x, pos.y, rect.GetWidth() / maxValue * value, rect.GetHeight());
}

void Hydro::ProgressBar::IncreaseValue(short add)
{
	value += add;
	if (value > maxValue)
	{
		value = maxValue;
	}
	inner.SetParameters(pos.x, pos.y, rect.GetWidth()/ maxValue * value, rect.GetHeight());
}

void Hydro::ProgressBar::DecreaseValue(short dec)
{
	value -= dec;
	if (value < 0)
	{
		value = 0;
	}
	inner.SetParameters(pos.x, pos.y, 0, rect.GetHeight());
}

void Hydro::ProgressBar::SetMaxValue(short _maxValue)
{
	maxValue = _maxValue;
	if (value > maxValue)
	{
		value = maxValue;
	}
	inner.SetParameters(pos.x, pos.y, rect.GetWidth() / maxValue * value, rect.GetHeight());
}

short Hydro::ProgressBar::GetMaxValue() const
{
	return maxValue;
}
