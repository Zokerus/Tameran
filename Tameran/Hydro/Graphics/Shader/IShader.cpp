#include "IShader.h"

Hydro::IShader::IShader(std::wstring shaderName)
	: pVertexShader(nullptr), pPixelShader(nullptr), pLayout(nullptr), matrixBuffer(nullptr), shaderName(shaderName)
{
}

Hydro::IShader::~IShader()
{
	//Release matrix buffer
	if (matrixBuffer)
	{
		matrixBuffer->Release();
		matrixBuffer = nullptr;
	}

	//Release vertex layout
	if (pLayout)
	{
		pLayout->Release();
		pLayout = nullptr;
	}

	//Release pixel shader
	if(pPixelShader)
	{
		pPixelShader->Release();
		pPixelShader = nullptr;
	}

	//Release vertex shader
	if (pVertexShader)
	{
		pVertexShader->Release();
		pVertexShader = nullptr;
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
