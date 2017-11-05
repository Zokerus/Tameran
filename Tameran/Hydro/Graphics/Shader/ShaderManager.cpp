#include "ShaderManager.h"

Hydro::ShaderManager::ShaderManager()
	: m_ready(false), m_textureShader()
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

	//Shader manager is ready to work
	m_ready = true;

	return true;
}

void Hydro::ShaderManager::Shutdown()
{
	//Shutdown the texture shader
	m_textureShader.Shutdown();
}

bool Hydro::ShaderManager::RenderTextureShader(ID3D11DeviceContext * deviceContext, int indexCount, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX projection, ID3D11ShaderResourceView * texture)
{
	//Render the texture shader
	if (!m_textureShader.Render(deviceContext, indexCount, world, view, projection, texture))
	{
		return false;
	}

	return true;
}
