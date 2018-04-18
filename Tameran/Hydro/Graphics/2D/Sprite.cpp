#include "Sprite.h"

Hydro::Sprite::Sprite()
	: m_vertexBuffer(nullptr), m_indexBuffer(nullptr), m_vertexCount(0), m_indexCount(0), m_texture(nullptr), m_ownTexture(false), m_screenWidth(0), m_screenHeight(0),
	m_imageWidth(0), m_imageHeight(0), m_xPrevPosition(0), m_yPrevPosition(0), m_ready(false)
{}

Hydro::Sprite::~Sprite()
{
	if (m_ready)
	{
		Shutdown();
	}
}

bool Hydro::Sprite::Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext, int screenWidth, int screenHeight, std::string textureFilename, int imageWidth, int imageHeight)
{
	bool result;

	//Initialize the parameters of the sprite object
	InitializeParameters(screenWidth, screenHeight, imageWidth, imageHeight);

	//Initialize the vertex and the index buffers
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}
	//Load the texture for this sprite
	result = LoadTexture(device, deviceContext, textureFilename);
	if (!result)
	{
		return false;
	}
	return true;
}

bool Hydro::Sprite::Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext, int screenWidth, int screenHeight, std::string textureFilename)
{
	bool result;

	//Use the standard initializer with a image width and height of 0
	result = Initialize(device, deviceContext, screenWidth, screenHeight, textureFilename, 0, 0);
	if (!result)
	{
		return false;
	}
	//Get the actual width and height of the texture
	m_imageWidth = m_texture->GetTextureWidth();
	m_imageHeight = m_texture->GetTextureHeight();
	if (m_imageWidth <= 0 || m_imageHeight <= 0)
	{
		return false;
	}
	return true;
}

bool Hydro::Sprite::Initialize(ID3D11Device * device, int screenWidth, int screenHeight, Texture * texture, int imageWidth, int imageHeight)
{
	bool result;

	//Initialize the parameters of the sprite object
	InitializeParameters(screenWidth, screenHeight, imageWidth, imageHeight);

	//Initialize the vertex and the index buffers
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}
	//Store texture of the sprite as a pointer
	m_texture = texture;

	return true;
}

bool Hydro::Sprite::Initialize(ID3D11Device * device, int screenWidth, int screenHeight, Texture * texture)
{
	//Use the standard initializer and the actual width and height of the texture
	if (!Initialize(device, screenWidth, screenHeight, texture, texture->GetTextureWidth(), texture->GetTextureHeight()))
	{
		return false;
	}
	return true;
}

void Hydro::Sprite::Shutdown()
{
	//Release the sprite texture
	ReleaseTexture();

	//Shutdown the vertex an the index buffers
	ShutdownBuffers();
}

bool Hydro::Sprite::Update(ID3D11DeviceContext * deviceContext, int xPosition, int yPosition)
{
	float left, right, top, bottom;
	VertexType *vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType *verticesPtr;
	HRESULT result;

	//Check the position of the sprite, if no change has occured, no update is needed, bc the vertex buffer has the right parameters
	if (m_xPrevPosition == xPosition && m_yPrevPosition == yPosition)
	{
		return true;
	}

	//update the prevposition to the new position
	m_xPrevPosition = xPosition;
	m_yPrevPosition = yPosition;

	//Calculate the screenCoords of left side of the sprite
	left = (float)((m_screenWidth / 2) * -1) + (float)xPosition;

	//Calculate the screen coordinates of the right side of the bitmap.
	right = left + (float)m_imageWidth;

	//Calculate the screen coordinates of the top of the bitmap.
	top = (float)(m_screenHeight / 2) - (float)yPosition;

	//Calculate the screen coordinates of the bottom of the bitmap.
	bottom = top - (float)m_imageHeight;

	//Create the vertex array
	vertices = new VertexType[m_vertexCount];
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
	result = deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the vertex buffer.
	verticesPtr = (VertexType*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * m_vertexCount));

	// Unlock the vertex buffer.
	deviceContext->Unmap(m_vertexBuffer, 0);

	// Release the vertex array as it is no longer needed.
	delete[] vertices;
	vertices = 0;

	return true;
}

bool Hydro::Sprite::Render(ID3D11DeviceContext * deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return true;
}

void Hydro::Sprite::SetTexture(Texture* texture)
{
	//Release old Texture if needed
	ReleaseTexture();

	//Store new texture
	m_texture = texture;
}

int Hydro::Sprite::GetIndexCount() const
{
	return m_indexCount;
}

ID3D11ShaderResourceView* Hydro::Sprite::GetTexture() const
{
	return m_texture->GetTexture();
}

int Hydro::Sprite::GetImageWidth() const
{
	return m_imageWidth;
}

int Hydro::Sprite::GetImageHeight() const
{
	return m_imageHeight;
}

void Hydro::Sprite::InitializeParameters(int screenWidth, int screenHeight, int imageWidth, int imageHeight)
{
	//Store the screen size
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	//Store the size of the image in pixels
	m_imageWidth = imageWidth;
	m_imageHeight = imageHeight;

	//Initialize the previous rendering position
	m_xPrevPosition = -1;
	m_yPrevPosition = -1;
}

bool Hydro::Sprite::InitializeBuffers(ID3D11Device* device)
{
	VertexType *vertices;
	unsigned long *indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	// Set the number of vertices in the vertex array.
	m_vertexCount = 4;

	// Set the number of indices in the index array.
	m_indexCount = 6;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType) * m_vertexCount));

	// Load the index array with data.
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 1;
	indices[4] = 3;
	indices[5] = 2;


	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
		return false;

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void Hydro::Sprite::ShutdownBuffers()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = nullptr;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = nullptr;
	}
}

bool Hydro::Sprite::LoadTexture(ID3D11Device * device, ID3D11DeviceContext * deviceContext, std::string textureFilename)
{
	// Create the texture object.
	m_texture = new Texture(device, deviceContext, textureFilename);
	if (!m_texture)
	{
		return false;
	}

	//Flag that the texture is loaded by this sprite and must be destroyed at the end
	m_ownTexture = true;

	return true;
}

void Hydro::Sprite::ReleaseTexture()
{
	// Release the texture object.
	if (m_texture && m_ownTexture)
	{
		delete m_texture;
		m_texture = nullptr;
	}
}

