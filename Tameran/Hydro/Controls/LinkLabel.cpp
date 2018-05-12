#include "LinkLabel.h"

Hydro::LinkLabel::LinkLabel(Direct3D* direct3D, Font* font, int ScreenWidth, int ScreenHeight, int MaxLength, std::string Name, std::string text, DirectX::XMINT2 Pos, DirectX::XMFLOAT2 Size, DirectX::XMVECTORF32 Color, DirectX::XMVECTORF32 selectedColor, std::function<void(IControl*)> func)
	: Label(direct3D, font, ScreenWidth, ScreenHeight, MaxLength, Name, text, Pos, Size, Color), selectedColor(selectedColor), counter(0), select(func)
{
	tabstop = true;
}

Hydro::LinkLabel::~LinkLabel()
{
}

bool Hydro::LinkLabel::Update(float eTime)
{
	return true;
}

bool Hydro::LinkLabel::Draw(float eTime, ID3D11DeviceContext* deviceContext, ShaderManager* shaderManager, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX orthoMatrix)
{

	if(focus)
	{
		if (!string.Render(deviceContext, shaderManager, worldMatrix, viewMatrix, orthoMatrix, selectedColor))
		{
			return false;
		}
	}
	else
	{
		if (!string.Render(deviceContext, shaderManager, worldMatrix, viewMatrix, orthoMatrix))
		{
			return false;
		}
	}

	return true;
}

void Hydro::LinkLabel::HandleInput(Input* input)
{
	if (!focus)
	{
		return;
	}

	if (input->IsKeyReleased(DIK_RETURN) || input->IsKeyReleased(DIK_NUMPADENTER))
	{
		select(this);
	}
}
