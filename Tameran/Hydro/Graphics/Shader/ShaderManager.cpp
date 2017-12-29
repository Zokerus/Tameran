#include "ShaderManager.h"

Hydro::ShaderManager::ShaderManager()
	: m_ready(false), m_textureShader(), m_fontShader()
{}

Hydro::ShaderManager::~ShaderManager()
{
	if (m_ready)
	{
		Shutdown();
	}
}

bool Hydro::ShaderManager::Initialize(ID3D11Device* device, HWND hWnd)
{
	bool result;

	//Initialize the texture shader
	result = m_textureShader.Initialize(device, hWnd);
	if (!result)
	{
		MessageBox(hWnd, "Could not initialize the texture shader object.", "Error", MB_OK);
		return false;
	}

	//Initialize the font shader
	result = m_fontShader.Initialize(device, hWnd);
	if (!result)
	{
		MessageBox(hWnd, "Could not initialize the font shader object.", "Error", MB_OK);
		return false;
	}

	//Shader manager is ready to work
	m_ready = true;

	return true;
}

void Hydro::ShaderManager::Shutdown()
{
	//Shutdown font shader
	m_fontShader.Shutdown();

	//Shutdown the texture shader
	m_textureShader.Shutdown();
}

bool Hydro::ShaderManager::RenderTextureShader(ID3D11DeviceContext* deviceContext, int indexCount, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX projection, ID3D11ShaderResourceView *texture)
{
	//Render the texture shader
	if (!m_textureShader.Render(deviceContext, indexCount, world, view, projection, texture))
	{
		return false;
	}

	return true;
}

bool Hydro::ShaderManager::RenderFontShader(ID3D11DeviceContext * deviceContext, int indexCount, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX ortho, ID3D11ShaderResourceView * texture, DirectX::XMVECTORF32 color)
{
	//Render the font shader object
	if(!m_fontShader.Render(deviceContext, indexCount, world, view, ortho, texture, color))
	{
		return false;
	}

	return true;
}
