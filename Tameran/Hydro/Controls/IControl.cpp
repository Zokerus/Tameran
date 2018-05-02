#include "IControl.h"

Hydro::IControl::IControl(std::string Name, DirectX::XMFLOAT2 Size, DirectX::XMINT2 Pos, bool Focus, bool Enable, bool Visible, bool Tabstop, DirectX::XMVECTORF32 Color, ControlType Type)
	: name(Name), size(Size), pos(Pos), focus(Focus), enable(Enable), visible(Visible), tabstop(Tabstop),	color(Color), type(Type)
{}

Hydro::IControl::~IControl()
{
}

bool Hydro::IControl::Update(float eTime)
{
	return true;
}

bool Hydro::IControl::Draw(float eTime, ID3D11DeviceContext* deviceContext, ShaderManager* shaderManager, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX orthoMatrix)
{
	return true;
}

void Hydro::IControl::HandleInput(Input* input)
{
}

void Hydro::IControl::SetFocus(bool Focus)
{
	focus = Focus;
}

void Hydro::IControl::SetStatus(bool Enable)
{
	enable = Enable;
}

void Hydro::IControl::Visibility(bool Visibility)
{
	visible = Visibility;
}

void Hydro::IControl::SetTabStop(bool TabStop)
{
	tabstop = TabStop;
}

void Hydro::IControl::SetPosition(DirectX::XMINT2 Pos)
{
	pos = Pos;
}

void Hydro::IControl::SetSize(DirectX::XMFLOAT2 Size)
{
	size = Size;
}

std::string Hydro::IControl::GetName() const
{
	return name;
}

DirectX::XMFLOAT2 Hydro::IControl::GetSize() const
{
	return size;
}

DirectX::XMINT2 Hydro::IControl::GetPosition() const
{
	return pos;
}

bool Hydro::IControl::HasFocus() const
{
	return focus;
}

bool Hydro::IControl::IsEnable() const
{
	return enable;
}

bool Hydro::IControl::IsVisible() const
{
	return visible;
}

bool Hydro::IControl::GetTabStop() const
{
	return tabstop;
}

DirectX::XMVECTORF32 Hydro::IControl::GetColor() const
{
	return color;
}

Hydro::IControl::ControlType Hydro::IControl::GetType() const
{
	return type;
}