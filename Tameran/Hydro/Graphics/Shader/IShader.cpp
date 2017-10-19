#include "IShader.h"

Hydro::IShader::IShader()
	: m_vertexShader(nullptr), m_pixelShader(nullptr), m_layout(nullptr), m_matrixBuffer(nullptr), m_ready(false)
{
}

Hydro::IShader::~IShader()
{
	if (m_ready)
	{
		Shutdown();
	}
}

bool Hydro::IShader::Initialize(ID3D11Device * device, HWND hWnd)
{
	HRESULT result;
	ID3DBlob* errorMessage(nullptr);
	ID3DBlob* vertexShaderBuffer(nullptr);
	ID3DBlob* pixelShaderBuffer(nullptr);
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;

	return true;
}
