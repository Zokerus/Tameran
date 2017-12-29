#include "IControl.h"

Hydro::IControl::IControl()
	: m_name(nullptr), m_text(nullptr), m_size(), m_pos(), m_focus(false), m_enable(true), m_visible(true), m_tabstop(false), m_ready(false),
	m_color(), m_type()
{}

Hydro::IControl::~IControl()
{
	if (m_ready)
	{
		Shutdown();
	}
}

bool Hydro::IControl::Initialize()
{
	return true;
}

void Hydro::IControl::Shutdown()
{
	//Shutdown the name field
	if (m_name)
	{
		delete m_name;
		m_name = nullptr;
	}

	//Shutdown the text field
	if (m_text)
	{
		delete m_text;
		m_text = nullptr;
	}
}

bool Hydro::IControl::Update(float eTime)
{
	return true;
}

bool Hydro::IControl::Draw(float eTime, ID3D11DeviceContext * deviceContext, ShaderManager * shaderManager, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX orthoMatrix)
{
	return true;
}

void Hydro::IControl::HandleInput(Input * input)
{
}

void Hydro::IControl::SetFocus(bool focus)
{
	m_focus = focus;
}

char* Hydro::IControl::GetName() const
{
	return m_name;
}

char* Hydro::IControl::GetString() const
{
	return m_text;
}

DirectX::XMFLOAT2 Hydro::IControl::GetSize() const
{
	return m_size;
}

DirectX::XMINT2 Hydro::IControl::GetPosition() const
{
	return m_pos;
}

bool Hydro::IControl::HasFocus() const
{
	return m_focus;
}

bool Hydro::IControl::IsEnable() const
{
	return m_enable;
}

bool Hydro::IControl::IsVisible() const
{
	return m_visible;
}

bool Hydro::IControl::GetTabStop() const
{
	return m_tabstop;
}

DirectX::XMVECTORF32 Hydro::IControl::GetColor() const
{
	return m_color;
}

Hydro::IControl::ControlType Hydro::IControl::GetType() const
{
	return m_type;
}