#include "Sprite.h"
#include <exception>

Hydro::Sprite::Sprite(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int ScreenWidth, int ScreenHeight, std::string TextureFilename, int ImageWidth, int ImageHeight)
	: pVertexBuffer(nullptr), pIndexBuffer(nullptr), vertexCount(0), indexCount(0), pTexture(nullptr), ownTexture(true), screenWidth(ScreenWidth), screenHeight(ScreenHeight),
	imageWidth(ImageWidth), imageHeight(ImageHeight), xPrevPosition(-1), yPrevPosition(-1)
{
	bool result = true;

	//Initialize the vertex and the index buffers
	result = InitializeBuffers(device);
	if (!result)
	{
		throw std::exception("Failed to initialize buffers from a sprite");
	}

	//Load the texture for this sprite
	result = LoadTexture(device, deviceContext, TextureFilename);
	if (!result)
	{
		throw std::exception(("Failed to load Texture: " + TextureFilename).c_str());
	}
}

Hydro::Sprite::Sprite(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, int _screenWidth, int _screenHeight, std::string _textureFilename, Rectangle& _srcRect)
	: pVertexBuffer(nullptr), pIndexBuffer(nullptr), vertexCount(0), indexCount(0), pTexture(nullptr), ownTexture(true), screenWidth(_screenWidth), screenHeight(_screenHeight),
	imageWidth(_srcRect.GetWidth()), imageHeight(_srcRect.GetHeight()), xPrevPosition(-1), yPrevPosition(-1)
{
	bool result = true;

	//Initialize the vertex and the index buffers
	result = InitializeBuffers(_device);
	if (!result)
	{
		throw std::exception("Failed to initialize buffers from a sprite");
	}

	//Load the texture for this sprite
	result = LoadTexture(_device, _deviceContext, _textureFilename, _srcRect);
	if (!result)
	{
		throw std::exception(("Failed to load Texture: " + _textureFilename).c_str());
	}
}

Hydro::Sprite::Sprite(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int ScreenWidth, int ScreenHeight, std::string TextureFilename)
	: Sprite(device, deviceContext, ScreenWidth, ScreenHeight, TextureFilename, 0, 0)
{
	//Get the actual width and height of the texture
	imageWidth = pTexture->GetTextureWidth();
	imageHeight = pTexture->GetTextureHeight();
	if (imageWidth <= 0 || imageHeight <= 0)
	{
		throw std::exception(("Error in texture data: " + TextureFilename).c_str());
	}
}

Hydro::Sprite::Sprite(ID3D11Device* device, int ScreenWidth, int ScreenHeight, Texture* texture, int ImageWidth, int ImageHeight)
	: pVertexBuffer(nullptr), pIndexBuffer(nullptr), vertexCount(0), indexCount(0), pTexture(texture), ownTexture(false), screenWidth(ScreenWidth), screenHeight(ScreenHeight),
	imageWidth(ImageWidth), imageHeight(ImageHeight), xPrevPosition(-1), yPrevPosition(-1)
{
	//Initialize the vertex and the index buffers
	if(!InitializeBuffers(device))
	{
		throw std::exception("Failed to initialize buffers from a sprite");
	}
}

Hydro::Sprite::Sprite(ID3D11Device* device, int ScreenWidth, int ScreenHeight, Texture* texture)
	: Sprite(device, ScreenWidth, ScreenHeight, texture, texture->GetTextureWidth(), texture->GetTextureHeight())
{}

Hydro::Sprite::~Sprite()
{
	//Release the sprite texture
	ReleaseTexture();

	//Shutdown the vertex an the index buffers
	ShutdownBuffers();
}

bool Hydro::Sprite::Update(ID3D11DeviceContext* _deviceContext, DirectX::XMINT2& _pos)
{
	return Update(_deviceContext, Rectangle(_pos.x, _pos.y, 0, 0));
}

bool Hydro::Sprite::Update(ID3D11DeviceContext* _deviceContext, Rectangle & _dstRect)
{
	float left, right, top, bottom;
	VertexType *vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType *verticesPtr;
	HRESULT result;

	//Check the position of the sprite, if no change has occured, no update is needed, bc the vertex buffer has the right parameters
	if (xPrevPosition == _dstRect.GetXPos() && yPrevPosition == _dstRect.GetYPos())
	{
		return true;
	}

	//update the prevposition to the new position
	xPrevPosition = _dstRect.GetXPos();
	yPrevPosition = _dstRect.GetYPos();

	//Calculate the screenCoords of left side of the sprite
	left = (float)((screenWidth / 2) * -1) + (float)_dstRect.GetXPos();

	//Calculate the screen coordinates of the right side of the bitmap.
	right = left + (float)imageWidth;

	//Calculate the screen coordinates of the top of the bitmap.
	top = (float)(screenHeight / 2) - (float)_dstRect.GetYPos();

	//Calculate the screen coordinates of the bottom of the bitmap.
	bottom = top - (float)imageHeight;

	//Create the vertex array
	vertices = new VertexType[vertexCount];
	if (!vertices)
	{
		return false;
	}

	vertices[0].position = DirectX::XMFLOAT3(left, bottom, 0.0f);  // Bottom left.
	vertices[0].texture = DirectX::XMFLOAT2(0.0f, 1.0f);

	vertices[1].position = DirectX::XMFLOAT3(left, top, 0.0f);  // Top left.
	vertices[1].texture = DirectX::XMFLOAT2(0.0f, 0.0f);

	vertices[2].position = DirectX::XMFLOAT3(right, bottom, 0.0f);  // Bottom right.
	vertices[2].texture = DirectX::XMFLOAT2(1.0f, 1.0f);

	vertices[3].position = DirectX::XMFLOAT3(right, top, 0.0f);  // Top right.
	vertices[3].texture = DirectX::XMFLOAT2(1.0f, 0.0f);


	// Lock the vertex buffer so it can be written to.
	result = _deviceContext->Map(pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the vertex buffer.
	verticesPtr = (VertexType*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * vertexCount));

	// Unlock the vertex buffer.
	_deviceContext->Unmap(pVertexBuffer, 0);

	// Release the vertex array as it is no longer needed.
	delete[] vertices;
	vertices = 0;

	return true;
}

bool Hydro::Sprite::Render(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return true;
}

void Hydro::Sprite::SetTexture(Texture* texture)
{
	//Release old Texture if needed
	ReleaseTexture();

	//Store new texture
	pTexture = texture;
}

int Hydro::Sprite::GetIndexCount() const
{
	return indexCount;
}

ID3D11ShaderResourceView* Hydro::Sprite::GetTexture() const
{
	return pTexture->GetTexture();
}

int Hydro::Sprite::GetImageWidth() const
{
	return imageWidth;
}

int Hydro::Sprite::GetImageHeight() const
{
	return imageHeight;
}

void Hydro::Sprite::InitializeParameters(int screenWidth, int screenHeight, int imageWidth, int imageHeight)
{
	//Store the screen size
	screenWidth = screenWidth;
	screenHeight = screenHeight;

	//Store the size of the image in pixels
	imageWidth = imageWidth;
	imageHeight = imageHeight;

	//Initialize the previous rendering position
	xPrevPosition = -1;
	yPrevPosition = -1;
}

bool Hydro::Sprite::InitializeBuffers(ID3D11Device* device)
{
	VertexType *vertices;
	unsigned long *indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	// Set the number of vertices in the vertex array.
	vertexCount = 4;

	// Set the number of indices in the index array.
	indexCount = 6;

	// Create the vertex array.
	vertices = new VertexType[vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[indexCount];
	if (!indices)
	{
		return false;
	}

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType) * vertexCount));

	// Load the index array with data.
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 1;
	indices[4] = 3;
	indices[5] = 2;


	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &pVertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &pIndexBuffer);
	if (FAILED(result))
		return false;

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;

	return true;
}

void Hydro::Sprite::ShutdownBuffers()
{
	// Release the index buffer.
	if (pIndexBuffer)
	{
		pIndexBuffer->Release();
		pIndexBuffer = nullptr;
	}

	// Release the vertex buffer.
	if (pVertexBuffer)
	{
		pVertexBuffer->Release();
		pVertexBuffer = nullptr;
	}
}

bool Hydro::Sprite::LoadTexture(ID3D11Device* device, ID3D11DeviceContext * deviceContext, std::string textureFilename, Rectangle& _srcRect)
{
	// Create the texture object.
	pTexture = new Texture(device, deviceContext, textureFilename, _srcRect);
	if (!pTexture)
	{
		return false;
	}

	//Flag that the texture is loaded by this sprite and must be destroyed at the end
	ownTexture = true;

	return true;
}

bool Hydro::Sprite::LoadTexture(ID3D11Device * device, ID3D11DeviceContext * deviceContext, std::string textureFilename)
{
	// Create the texture object.
	pTexture = new Texture(device, deviceContext, textureFilename);
	if (!pTexture)
	{
		return false;
	}

	//Flag that the texture is loaded by this sprite and must be destroyed at the end
	ownTexture = true;

	return true;
}

void Hydro::Sprite::ReleaseTexture()
{
	// Release the texture object.
	if (pTexture && ownTexture)
	{
		delete pTexture;
		pTexture = nullptr;
	}
}

