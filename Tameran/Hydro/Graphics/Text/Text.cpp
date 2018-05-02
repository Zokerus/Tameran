#include "Text.h"
#include <exception>

Hydro::Text::Text(Direct3D* direct3D, Font* inFont, int ScreenWidth, int ScreenHeight, int MaxLength, bool Shadow, const std::string* Text, DirectX::XMINT2 Pos, DirectX::XMVECTORF32 Color)
	: vertexBuffer(nullptr), indexBuffer(nullptr), vertexBuffer2(nullptr), indexBuffer2(nullptr),
	pixelColor(Color), pos(Pos), screenWidth(ScreenWidth), screenHeight(ScreenHeight), maxLength(MaxLength),
	vertexCount(0), indexCount(0), shadow(Shadow), update(false), font(inFont), text(*Text)
{
	Font::VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i, index, numLetters;

	// Get the number of letters in the sentence.
	numLetters = (int)Text->length();

	// Check for possible buffer overflow.
	if (numLetters > maxLength)
	{
		throw std::exception("Text length is to long");
	}

	//Set the vertex and index count
	vertexCount = maxLength * 4;
	indexCount = maxLength * 6;

	// Create the vertex array.
	vertices = new Font::VertexType[vertexCount];
	if (!vertices)
	{
		throw std::exception("Failed to create vertex array in text class");
	}

	// Create the index array.
	indices = new unsigned long[indexCount];
	if (!indices)
	{
		throw std::exception("Failed to create index array in text class");
	}

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(Font::VertexType) * vertexCount));

	//Initialize the index of the index array
	index = 0;

	// Initialize the index array.
	for (i = 0; i<maxLength; i++)
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
	vertexBufferDesc.ByteWidth = sizeof(Font::VertexType) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Create the vertex buffer.
	result = direct3D->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
	if (FAILED(result))
	{
		throw std::exception("Failed to create vertex buffer in text class");
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
	result = direct3D->GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
	if (FAILED(result))
	{
		throw std::exception("Failed to create index buffer in text class");
	}

	//If shadowed create the second vertex and index buffer
	if (shadow)
	{
		result = direct3D->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer2);
		if (FAILED(result))
		{
			throw std::exception("Failed to create vertex buffer of shadow in text class");
		}

		result = direct3D->GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer2);
		if (FAILED(result))
		{
			throw std::exception("Failed to create index buffer of shadow in text class");
		}
	}

	// Release the vertex array as it is no longer needed.
	delete[] vertices;
	vertices = nullptr;

	// Release the index array as it is no longer needed.
	delete[] indices;
	indices = nullptr;

	//Now add the text data to the sentece buffers
	result = UpdateSentence(direct3D->GetDeviceContext());
	if (!result)
	{
		throw std::exception("Failed to create Text object");
	}

}

Hydro::Text::~Text()
{
	font = nullptr;
	// Release the vertex and index buffers.
	if (indexBuffer2)
	{
		indexBuffer2->Release();
		indexBuffer2 = nullptr;
	}

	if (vertexBuffer2)
	{
		vertexBuffer2->Release();
		vertexBuffer2 = nullptr;
	}

	if (indexBuffer)
	{
		indexBuffer->Release();
		indexBuffer = nullptr;
	}

	if (vertexBuffer)
	{
		vertexBuffer->Release();
		vertexBuffer = nullptr;
	}
}

bool Hydro::Text::Render(ID3D11DeviceContext* deviceContext, ShaderManager* shaderManager, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX orthoMatrix)
{
	//Draw the sentence
	if (!RenderSentence(deviceContext, shaderManager, worldMatrix, viewMatrix, orthoMatrix, pixelColor))
	{
		return false;
	}

	return true;
}

bool Hydro::Text::Render(ID3D11DeviceContext* deviceContext, ShaderManager* shaderManager, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX orthoMatrix, DirectX::XMVECTORF32 foreColor)
{
	//Draw the sentence
	if(!RenderSentence(deviceContext, shaderManager, worldMatrix, viewMatrix, orthoMatrix, foreColor))
	{
		return false;
	}
	return true;
}

void Hydro::Text::SetText(const std::string* Text)
{
	text = *Text;
	update = true;
}

void Hydro::Text::SetPosition(DirectX::XMINT2 Pos)
{
	pos = Pos;
	update = true;
}

void Hydro::Text::SetFont(Font* inFont)
{
	font = inFont;
	update = true;
}

void Hydro::Text::SetColor(DirectX::XMVECTORF32 Color)
{
	pixelColor = Color;
	update = true;
}

bool Hydro::Text::UpdateSentence(ID3D11DeviceContext* deviceContext)
{
	Font::VertexType* vertices;
	float drawX, drawY;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	Font::VertexType* verticesPtr;

	// Create the vertex array.
	vertices = new Font::VertexType[vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(Font::VertexType) * vertexCount));

	// Calculate the X and Y pixel position on the screen to start drawing to.
	drawX = (float)(((screenWidth / 2) * -1) + pos.x);
	drawY = (float)((screenHeight / 2) - pos.y);

	// Use the font class to build the vertex array from the sentence text and sentence draw location.
	font->BuildVertexArray((void*)vertices, &text, drawX, drawY);

	// Lock the vertex buffer so it can be written to.
	result = deviceContext->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the vertex buffer.
	verticesPtr = (Font::VertexType*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)vertices, (sizeof(Font::VertexType) * vertexCount));

	// Unlock the vertex buffer.
	deviceContext->Unmap(vertexBuffer, 0);

	//If shadowed then do the same for the second vertex buffer but offset by two pixels on both axis
	//TODO shadow für den text einfügen
	if (shadow)
	{
		memset(vertices, 0, (sizeof(Font::VertexType) * vertexCount));

		drawX = (float)((((screenWidth / 2) * -1) + pos.x) + 2);
		drawY = (float)(((screenHeight / 2) - pos.y) - 2);
		font->BuildVertexArray((void*)vertices, &text, drawX, drawY);

		result = deviceContext->Map(vertexBuffer2, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(result))
		{
			return false;
		}
		
		verticesPtr = (Font::VertexType*)mappedResource.pData;
		memcpy(verticesPtr, (void*)vertices, (sizeof(Font::VertexType) * vertexCount));
		deviceContext->Unmap(vertexBuffer2, 0);
	}

	// Release the vertex array as it is no longer needed.
	delete[] vertices;
	vertices = 0;

	return true;
}

const Hydro::Font* Hydro::Text::GetFont() const
{
	return font;
}

const std::string* Hydro::Text::GetText() const
{
	return &text;
}

bool Hydro::Text::RenderSentence(ID3D11DeviceContext* deviceContext, ShaderManager* shaderManager, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX orthoMatrix, DirectX::XMVECTORF32 foreColor)
{
	unsigned int stride, offset;
	DirectX::XMVECTORF32 shadowColor;
	bool result;

	//Update text object if needed
	if (update)
	{
		if (!UpdateSentence(deviceContext))
		{
			return false;
		}
		update = false;
	}

	// Set vertex buffer stride and offset.
	stride = sizeof(Font::VertexType);
	offset = 0;

	//If shadowed then render the shadow text first
	if (shadow)
	{
		shadowColor = DirectX::Colors::Black;

		deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer2, &stride, &offset);
		deviceContext->IASetIndexBuffer(indexBuffer2, DXGI_FORMAT_R32_UINT, 0);
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		result = shaderManager->RenderFontShader(deviceContext, indexCount, worldMatrix, viewMatrix, orthoMatrix, font->GetTexture(), shadowColor);
		if (!result)
		{
			return false;
		}
	}

	//Render the text buffer
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Render the text using the font shader.
	result = shaderManager->RenderFontShader(deviceContext, indexCount, worldMatrix, viewMatrix, orthoMatrix, font->GetTexture(), foreColor);
	if (!result)
	{
		false;
	}

	return true;
}
