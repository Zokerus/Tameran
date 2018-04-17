#include "TextureShader.h"

Hydro::TextureShader::TextureShader(ID3D11Device* device, HWND hWnd)
	: IShader(L"TextureShader")
{
	HRESULT result;
	ID3DBlob* errorMessage(nullptr);
	ID3DBlob* vertexShaderBuffer(nullptr);
	ID3DBlob* pixelShaderBuffer(nullptr);
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;

	//Load precompiled shader file
	std::wstring dir = (wDir + L"/Data/Shader/" + m_shaderName + L"/VertexShader.cso");
	result = D3DReadFileToBlob((wDir + L"/Data/Shader/" + m_shaderName + L"/VertexShader.cso").c_str(), &vertexShaderBuffer);
	if (FAILED(result))
	{
		throw std::exception("Failed to load Vertexshader.cso of the TextureShader");
	}

	//Create the vertex shader from buffer
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result))
	{
		throw std::exception("Failed to create VertexShader of the TextureShader");
	}

	//Load precompiled shader file
	result = D3DReadFileToBlob((wDir + L"/Data/Shader/" + m_shaderName + L"/PixelShader.cso").c_str(), &pixelShaderBuffer);
	if (FAILED(result))
	{
		throw std::exception("Failed to load Pixelshader.cso of the TextureShader");
	}

	//Create the pixel shader from buffer
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result))
	{
		throw std::exception("Failed to create PixelShader of the TextureShader");
	}

	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &m_layout);
	if (FAILED(result))
	{
		throw std::exception("Failed to create InputLayout of TextureShader");
	}

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = nullptr;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = nullptr;

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result))
	{
		throw std::exception("Failed to create MatrixBuffer in TextureShader");
	}

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
	if (FAILED(result))
	{
		throw std::exception("Failed to create SampleState in TextureShader");
	}
}

Hydro::TextureShader::~TextureShader()
{
	//Release the sample state
	if (m_sampleState)
	{
		m_sampleState->Release();
		m_sampleState = nullptr;
	}
}

bool Hydro::TextureShader::Render(ID3D11DeviceContext * deviceContext, int indexCount, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX projection, ID3D11ShaderResourceView * texture)
{
	//Set the shader parameters
	if (!SetShaderParameters(deviceContext, world, view, projection, texture))
	{
		return false;
	}

	//Set the vertex input layout
	deviceContext->IASetInputLayout(m_layout);

	//Set the vertex and pixel shaders
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	//Set the sampler state in the pixel shader
	deviceContext->PSSetSamplers(0, 1, &m_sampleState);

	//Render the triangle
	deviceContext->DrawIndexed(indexCount, 0, 0);

	return true;
}

bool Hydro::TextureShader::SetShaderParameters(ID3D11DeviceContext * deviceContext, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX projection, ID3D11ShaderResourceView * texture)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;

	// Transpose the matrices to prepare them for the shader.
	world = XMMatrixTranspose(world);
	view = XMMatrixTranspose(view);
	projection = XMMatrixTranspose(projection);

	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr->world = world;
	dataPtr->view = view;
	dataPtr->projection = projection;

	// Unlock the constant buffer.
	deviceContext->Unmap(m_matrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Finanly set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	//Set shader texture resource in the pixel shader
	deviceContext->PSSetShaderResources(0, 1, &texture);
	return true;
}
