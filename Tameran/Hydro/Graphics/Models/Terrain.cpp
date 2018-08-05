#include "Terrain.h"

Hydro::Terrain::Terrain(Direct3D* _direct3D)
	: pVertexBuffer(nullptr), pIndexBuffer(nullptr), vertexCount(0), indexCount(0)
{
	if (!InitializeBuffers(_direct3D->GetDevice()))
	{
		throw std::exception("Error while loading terrain");
	}
}

Hydro::Terrain::~Terrain()
{
	ShutdownBuffers();
}

bool Hydro::Terrain::Update(float eTime)
{
	return true;
}

bool Hydro::Terrain::Draw(ID3D11DeviceContext * deviceContext, float eTime)
{
	unsigned int stride;
	unsigned int offset;

	//Set the vertex buffer stride and offset
	stride = sizeof(VertexType);
	offset = 0;

	//Set the vertex buffer to active in the input assembler so it can be rendered
	deviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

	//Set the index buffer to active in the input assembler so it can be rendered
	deviceContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//Set the type of primitive that should be rendered from this vertex buffer, in this case trianglelist.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	return true;
}

unsigned int Hydro::Terrain::GetIndexCount()
{
	return indexCount;
}

bool Hydro::Terrain::InitializeBuffers(ID3D11Device * device)
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	VertexType *vertices;
	unsigned int *indices;
	unsigned int index = 0;

	vertexCount = terrainDimensions * terrainDimensions;
	indexCount = (terrainDimensions - 1) * (terrainDimensions - 1) * 6;

	vertices = new VertexType[vertexCount];
	indices = new unsigned int[indexCount];


	
	for (int z = 0; z < terrainDimensions - 1; z++)
	{
		for (int x = 0; x < terrainDimensions - 1; x++)
		{
			vertices[z * terrainDimensions + x].position = DirectX::XMFLOAT3((float)x - 50.f, 0.0f, (float)z- 50.0f);
			vertices[z * terrainDimensions + x].color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

			int lowerLeft = x + z * terrainDimensions;
			int lowerRight = (x + 1) + z * terrainDimensions;
			int topLeft = x + (z + 1) * terrainDimensions;
			int topRight = (x + 1) + (z + 1) * terrainDimensions;

			indices[index++] = lowerLeft;
			indices[index++] = topLeft;
			indices[index++] = topRight;

			indices[index++] = lowerLeft;
			indices[index++] = topRight;
			indices[index++] = lowerRight;
		}
		vertices[z * terrainDimensions + terrainDimensions - 1].position = DirectX::XMFLOAT3((float)(terrainDimensions - 1) - 50.f, 0.0f, (float)z - 50.0f);
		vertices[z * terrainDimensions + terrainDimensions - 1].color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	for (int x = 0; x < terrainDimensions; x++)
	{
		vertices[x + terrainDimensions * (terrainDimensions - 1)].position = DirectX::XMFLOAT3((float)x - 50.f, 0.0f, (float)(terrainDimensions - 1) - 50.0f);
		vertices[x + terrainDimensions * (terrainDimensions - 1)].color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	vertices[60 * terrainDimensions + 50].color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	//Give the subresource structure a pointer to the vertex data
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	//Now create the vertex buffer
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &pVertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * indexCount;
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
	{
		return false;
	}

	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;

	return true;
}

void Hydro::Terrain::ShutdownBuffers()
{
	if (pVertexBuffer)
	{
		pVertexBuffer->Release();
		pVertexBuffer = nullptr;
	}

	if (pIndexBuffer)
	{
		pIndexBuffer->Release();
		pIndexBuffer = nullptr;
	}
}
