#include "Label.h"

Hydro::Label::Label(Direct3D* direct3D, Font* font, int ScreenWidth, int ScreenHeight, int MaxLength, std::string Name, std::string text, DirectX::XMINT2 Pos, DirectX::XMFLOAT2 Size, DirectX::XMVECTORF32 Color)
	: IControl(Name, Size, Pos, false, true, true, false, Color, ControlType::Lablel),
	string(direct3D, font, ScreenWidth, ScreenHeight, MaxLength, true, &text, Pos, Color)
{}

Hydro::Label::~Label()
{}

bool Hydro::Label::Update(float eTime)
{
	return true;
}

bool Hydro::Label::Draw(float eTime, ID3D11DeviceContext* deviceContext, ShaderManager* shaderManager, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX orthoMatrix)
{
	if (!string.Render(deviceContext, shaderManager, worldMatrix, viewMatrix, orthoMatrix))
	{
		return false;
	}

	return true;
}

void Hydro::Label::HandleInput()
{
}

void Hydro::Label::SetPosition(DirectX::XMINT2 Pos)
{
	pos = Pos;
	string.SetPosition(Pos);
}

DirectX::XMFLOAT2 Hydro::Label::GetItemSize()
{
	return DirectX::XMFLOAT2(string.GetFont()->GetTextWidth(string.GetText()), string.GetFont()->GetFontHeight());
}

const std::string* Hydro::Label::GetString() const
{
	return string.GetText();
}
