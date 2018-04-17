#include "Text.h"

Hydro::Text::Text(int screenWidth, int screenHeight, int maxLength)
	: m_vertexBuffer(nullptr), m_indexBuffer(nullptr), m_vertexBuffer2(nullptr), m_indexBuffer2(nullptr),
	m_pixelColor(), m_pos(), m_screenWidth(screenWidth), m_screenHeight(screenHeight), m_maxLength(maxLength),
	m_vertexCount(0), m_indexCount(0), m_shadow(false), m_ready(false)
{
}

Hydro::Text::~Text()
{
	if (m_ready)
	{
		Shutdown();
	}
}

bool Hydro::Text::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, bool shadow, Font* font, const char* text, DirectX::XMINT2 pos, DirectX::XMVECTORF32 color)
{
	Font::VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i, index;

	//Store if this sentence is shadowed or not
	m_shadow = shadow;

	//Set the vertex and index count
	m_vertexCount = m_maxLength * 4;
	m_indexCount = m_maxLength * 6;

	// Create the vertex array.
	vertices = new Font::VertexType[m_vertexCount];
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
	memset(vertices, 0, (sizeof(Font::VertexType) * m_vertexCount));

	//Initialize the index of the index array
	index = 0;

	// Initialize the index array.
	for (i = 0; i<m_maxLength; i++)
	{
		indices[index++] = i * 4;
		indices[index++] = i * 4 + 1;
		indices[index++] = i * 4 + 2;

		indices[index++] = i * 4 + 1;
		indices[index++] = i * 4 + 3;
		indices[index++] = i * 4 + 2;
	}

	// Set up the description of the dynamic vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(Font::VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Create the vertex buffer.
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
	{
		return false;
	}

	//If shadowed create the second vertex and index buffer
	if (m_shadow)
	{
		result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer2);
		if (FAILED(result))
		{
			return false;
		}

		result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer2);
		if (FAILED(result))
		{
			return false;
		}
	}

	// Release the vertex array as it is no longer needed.
	delete[] vertices;
	vertices = 0;

	// Release the index array as it is no longer needed.
	delete[] indices;
	indices = 0;

	//Now add the text data to the sentece buffers
	result = UpdateSentence(deviceContext, font, text, pos, color);
	if (!result)
	{
		return false;
	}

	m_ready = true;

	return true;
}

void Hydro::Text::Shutdown()
{
	// Release the vertex and index buffers.
	if (m_indexBuffer2)
	{
		m_indexBuffer2->Release();
		m_indexBuffer2 = 0;
	}

	if (m_vertexBuffer2)
	{
		m_vertexBuffer2->Release();
		m_vertexBuffer2 = 0;
	}

	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	m_ready = false;
}

bool Hydro::Text::Render(ID3D11DeviceContext* deviceContext, ShaderManager* shaderManager, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX orthoMatrix, ID3D11ShaderResourceView* fontTexture)
{
	//Draw the sentence
	if (!RenderSentence(deviceContext, shaderManager, worldMatrix, viewMatrix, orthoMatrix, fontTexture, m_pixelColor))
	{
		return false;
	}

	return true;
}

bool Hydro::Text::Render(ID3D11DeviceContext * deviceContext, ShaderManager * shaderManager, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX orthoMatrix, ID3D11ShaderResourceView * fontTexture, DirectX::XMVECTORF32 foreColor)
{
	//Draw the sentence
	if(!RenderSentence(deviceContext, shaderManager, worldMatrix, viewMatrix, orthoMatrix, fontTexture, foreColor))
	{
		return false;
	}
	return true;
}

bool Hydro::Text::UpdateSentence(ID3D11DeviceContext * deviceContext, Font * font, const char * text, DirectX::XMINT2 pos, DirectX::XMVECTORF32 color)
{
	int numLetters;
	Font::VertexType* vertices;
	float drawX, drawY;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	Font::VertexType* verticesPtr;

	//Store the position of the sentence
	m_pos = pos;

	// Store the color of the sentence.
	m_pixelColor = color;

	// Get the number of letters in the sentence.
	numLetters = (int)strlen(text);

	// Check for possible buffer overflow.
	if (numLetters > m_maxLength)
	{
		return false;
	}

	// Create the vertex array.
	vertices = new Font::VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(Font::VertexType) * m_vertexCount));

	// Calculate the X and Y pixel position on the screen to start drawing to.
	drawX = (float)(((m_screenWidth / 2) * -1) + pos.x);
	drawY = (float)((m_screenHeight / 2) - pos.y);

	// Use the font class to build the vertex array from the sentence text and sentence draw location.
	font->BuildVertexArray((void*)vertices, text, drawX, drawY);

	// Lock the vertex buffer so it can be written to.
	result = deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the vertex buffer.
	verticesPtr = (Font::VertexType*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)vertices, (sizeof(Font::VertexType) * m_vertexCount));

	// Unlock the vertex buffer.
	deviceContext->Unmap(m_vertexBuffer, 0);

	//If shadowed then do the same for the second vertex buffer but offset by two pixels on both axis
	//TODO shadow für den text einfügen
	if (m_shadow)
	{
		memset(vertices, 0, (sizeof(Font::VertexType) * m_vertexCount));

		drawX = (float)((((m_screenWidth / 2) * -1) + pos.x) + 2);
		drawY = (float)(((m_screenHeight / 2) - pos.y) - 2);
		font->BuildVertexArray((void*)vertices, text, drawX, drawY);

		result = deviceContext->Map(m_vertexBuffer2, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(result))
			return false;
		verticesPtr = (Font::VertexType*)mappedResource.pData;
		memcpy(verticesPtr, (void*)vertices, (sizeof(Font::VertexType) * m_vertexCount));
		deviceContext->Unmap(m_vertexBuffer2, 0);
	}

	// Release the vertex array as it is no longer needed.
	delete[] vertices;
	vertices = 0;

	return true;
}

bool Hydro::Text::RenderSentence(ID3D11DeviceContext* deviceContext, ShaderManager* shaderManager, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX orthoMatrix, ID3D11ShaderResourceView * fontTexture, DirectX::XMVECTORF32 foreColor)
{
	unsigned int stride, offset;
	DirectX::XMVECTORF32 shadowColor;
	bool result;


	// Set vertex buffer stride and offset.
	stride = sizeof(Font::VertexType);
	offset = 0;

	//If shadowed then render the shadow text first
	if (m_shadow)
	{
		shadowColor = DirectX::Colors::Black;

		deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer2, &stride, &offset);
		deviceContext->IASetIndexBuffer(m_indexBuffer2, DXGI_FORMAT_R32_UINT, 0);
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		result = shaderManager->RenderFontShader(deviceContext, m_indexCount, worldMatrix, viewMatrix, orthoMatrix, fontTexture, shadowColor);
		if (!result)
			return false;
	}

	//Render the text buffer
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Render the text using the font shader.
	result = shaderManager->RenderFontShader(deviceContext, m_indexCount, worldMatrix, viewMatrix, orthoMatrix, fontTexture, foreColor);
	if (!result)
		false;

	return true;
}
