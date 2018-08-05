#include "ColorShader.h"

Hydro::ColorShader::ColorShader(ID3D11Device * device, HWND hWnd)
	: IShader(L"ColorShader")
{
	HRESULT result;
	ID3DBlob* errorMessage(nullptr);
	ID3DBlob* vertexShaderBuffer(nullptr);
	ID3DBlob* pixelShaderBuffer(nullptr);
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;


	//Load precompiled shader file
	result = D3DReadFileToBlob((L"Data/Shader/" + shaderName + L"/VertexShader.cso").c_str(), &vertexShaderBuffer);
	if (FAILED(result))
	{
		throw std::exception("Failed to load Vertexshader.cso of the Colorshader");
	}

	//Create the vertex shader from buffer
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &pVertexShader);
	if (FAILED(result))
	{
		throw std::exception("Failed to create VertexShader of the Colorshader");
	}

	//Load precompiled shader file
	result = D3DReadFileToBlob((L"Data/Shader/" + shaderName + L"/PixelShader.cso").c_str(), &pixelShaderBuffer);
	if (FAILED(result))
	{
		throw std::exception("Failed to load Pixelshader.cso of the Colorshader");
	}

	//Create the pixel shader from buffer
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &pPixelShader);
	if (FAILED(result))
	{
		throw std::exception("Failed to create PixelShader of the Colorshader");
	}

	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &pLayout);
	if (FAILED(result))
	{
		throw std::exception("Failed to create InputLayout of FontShader");
	}

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &matrixBuffer);
	if (FAILED(result))
	{
		throw std::exception("Failed to create MatrixBuffer in FontShader");
	}
}

Hydro::ColorShader::~ColorShader()
{
}

bool Hydro::ColorShader::Render(ID3D11DeviceContext * deviceContext, int indexCount, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX projection)
{
	//Set the shader parameters
	if (!SetShaderParameters(deviceContext, world, view, projection))
	{
		return false;
	}

	//Set the vertex input layout
	deviceContext->IASetInputLayout(pLayout);

	//Set the vertex and pixel shaders that will be used to render this triangle
	deviceContext->VSSetShader(pVertexShader, NULL, 0);
	deviceContext->PSSetShader(pPixelShader, NULL, 0);

	//Render the triangle
	deviceContext->DrawIndexed(indexCount, 0, 0);
	return true;
}

bool Hydro::ColorShader::SetShaderParameters(ID3D11DeviceContext * deviceContext, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX projection)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType *dataPtr;
	unsigned int bufferNumber;

	// Transpose the matrices to prepare them for the shader.
	world = XMMatrixTranspose(world);
	view = XMMatrixTranspose(view);
	projection = XMMatrixTranspose(projection);

	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
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
	deviceContext->Unmap(matrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Finanly set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &matrixBuffer);

	return true;
}
