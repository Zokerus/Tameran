#include "IShader.h"

Hydro::IShader::IShader(std::wstring shaderName)
	: m_vertexShader(nullptr), m_pixelShader(nullptr), m_layout(nullptr), m_matrixBuffer(nullptr), m_shaderName(shaderName), m_ready(false)
{
}

Hydro::IShader::~IShader()
{
	if (m_ready)
	{
		Shutdown();
	}
}

bool Hydro::IShader::Initialize(ID3D11Device* device, HWND hWnd)
{
	HRESULT result;
	ID3DBlob* errorMessage(nullptr);
	ID3DBlob* vertexShaderBuffer(nullptr);
	ID3DBlob* pixelShaderBuffer(nullptr);
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;

	//Load precompiled shader file
	result = D3DReadFileToBlob((wDir + L"/Data/Shader" + m_shaderName + L"/VertexShader.cso").c_str(), &vertexShaderBuffer);
	if (FAILED(result))
	{
		MessageBox(hWnd, "Failed to load VertexShader", "Error", MB_OK);
		return false;
	}

	//Create the vertex shader from buffer
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result))
	{
		MessageBox(hWnd, "Failed to create VertexShader", "Error", MB_OK);
		return false;
	}

	//Load precompiled shader file
	result = D3DReadFileToBlob((wDir + L"/Data/Shader" + m_shaderName + L"/PixelShader.cso").c_str(), &pixelShaderBuffer);
	if (FAILED(result))
	{
		MessageBox(hWnd, "Failed to load PixelShader", "Error", MB_OK);
		return false;
	}

	//Create the pixel shader from buffer
	result = device->CreateVertexShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result))
	{
		MessageBox(hWnd, "Failed to create PixelShader", "Error", MB_OK);
		return false;
	}

	return true;
}

void Hydro::IShader::Shutdown()
{
	//Release matrix buffer
	if (m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = nullptr;
	}

	//Release vertex layout
	if (m_layout)
	{
		m_layout->Release();
		m_layout = nullptr;
	}

	//Release pixel shader
	if(m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = nullptr;
	}

	//Release vertex shader
	if (m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = nullptr;
	}
}

bool Hydro::IShader::Render(ID3D11DeviceContext * deviceContext, int indexCount, MatrixBufferType matrices)
{
	return true;
}

bool Hydro::IShader::SetShaderParameters(ID3D11DeviceContext * deviceContext, MatrixBufferType * matrices)
{
	return true;
}

void Hydro::IShader::RenderShader(ID3D11DeviceContext * deviceContext, int indexCount)
{}

void Hydro::IShader::OutputShaderErrorMessage(ID3DBlob* errorMessage, HWND hwnd, std::string shaderFilename)
{
	char* compileErrors;
	unsigned long long bufferSize, i;
	std::ofstream fout;

	//Get a pointer to the error message text buffer
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	//Get the length of the message
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for (i = 0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, "Error compiling shader.  Check shader-error.txt for message.", (LPCSTR)shaderFilename.c_str(), MB_OK);

	return;
}
