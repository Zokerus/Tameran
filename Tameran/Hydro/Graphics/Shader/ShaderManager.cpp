#include "ShaderManager.h"

Hydro::ShaderManager::ShaderManager(ID3D11Device* device, HWND hWnd)
	: textureShader(device, hWnd), fontShader(device, hWnd), colorShader(device, hWnd)
{}

bool Hydro::ShaderManager::RenderTextureShader(ID3D11DeviceContext* deviceContext, int indexCount, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX projection, ID3D11ShaderResourceView *texture)
{
	//Render the texture shader
	if (!textureShader.Render(deviceContext, indexCount, world, view, projection, texture))
	{
		return false;
	}

	return true;
}

bool Hydro::ShaderManager::RenderFontShader(ID3D11DeviceContext* deviceContext, int indexCount, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX ortho, ID3D11ShaderResourceView * texture, DirectX::XMVECTORF32 color)
{
	//Render the font shader object
	if(!fontShader.Render(deviceContext, indexCount, world, view, ortho, texture, color))
	{
		return false;
	}

	return true;
}

bool Hydro::ShaderManager::RenderColorShader(ID3D11DeviceContext* deviceContext, int indexCount, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX projection)
{
	if (!colorShader.Render(deviceContext, indexCount, world, view, projection))
	{
		return false;
	}
	return true;
}
