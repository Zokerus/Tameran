#include "LinkLabel.h"

Hydro::LinkLabel::LinkLabel(DirectX::XMVECTORF32 selectedColor, std::function<void(IControl*)> func, int screenWidth, int screenHeight, int maxLength)
	: Label(screenWidth, screenHeight, maxLength), m_selectedColor(selectedColor), m_counter(0), m_select(func)
{}

Hydro::LinkLabel::~LinkLabel()
{
	if (m_ready)
	{
		Shutdown();
	}
}

bool Hydro::LinkLabel::Update(float eTime)
{
	return true;
}

bool Hydro::LinkLabel::Draw(float eTime, ID3D11DeviceContext * deviceContext, ShaderManager * shaderManager, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX orthoMatrix)
{

	if(m_focus)
	{
		if (!m_string.Render(deviceContext, shaderManager, worldMatrix, viewMatrix, orthoMatrix, m_font->GetTexture(), m_selectedColor))
		{
			return false;
		}
	}
	else
	{
		if (!m_string.Render(deviceContext, shaderManager, worldMatrix, viewMatrix, orthoMatrix, m_font->GetTexture()))
		{
			return false;
		}
	}

	return true;
}

void Hydro::LinkLabel::HandleInput(Input * input)
{
	if (!m_focus)
	{
		return;
	}

	if (input->IsKeyReleased(DIK_RETURN) || input->IsKeyReleased(DIK_NUMPADENTER))
	{
		m_select(this);
	}
}
