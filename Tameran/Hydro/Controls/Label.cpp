#include "Label.h"

Hydro::Label::Label(int screenWidth, int screenHeight, int maxLength)
	: m_string(screenWidth, screenHeight, maxLength), m_font(nullptr), m_ready(false)
{}

Hydro::Label::~Label()
{
	if(m_ready)
	{
		Shutdown();
	}
}

bool Hydro::Label::Initialize(Direct3D* direct3D, Font* font, char* string, DirectX::XMINT2 pos, DirectX::XMFLOAT2 size, DirectX::XMVECTORF32 color)
{
	bool result;

	m_tabstop = false;
	m_text = string;
	m_size = size;
	m_pos = pos;
	m_color = color;
	m_font = font;

	result = m_string.Initialize(direct3D->GetDevice(), direct3D->GetDeviceContext(), false, font, m_text.c_str(), pos, color);
	if (!result)
	{
		return false;
	}

	m_enable = true;
	m_visible = true;

	m_ready = true;

	return true;
}

void Hydro::Label::Shutdown()
{
	m_font = nullptr;
	m_ready = false;
}

bool Hydro::Label::Update(float eTime)
{
	return true;
}

bool Hydro::Label::Draw(float eTime, ID3D11DeviceContext * deviceContext, ShaderManager * shaderManager, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX orthoMatrix)
{
	if (!m_string.Render(deviceContext, shaderManager, worldMatrix, viewMatrix, orthoMatrix, m_font->GetTexture()))
	{
		return false;
	}

	return true;
}

void Hydro::Label::HandleInput()
{
}