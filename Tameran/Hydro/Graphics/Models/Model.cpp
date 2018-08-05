#include "Model.h"
#include "../../Util/BinaryFile.h"

Hydro::Model::Model(Direct3D *_direct3D, const std::string modelfile)
	: pVertexBuffer(nullptr), pIndexBuffer(nullptr), vertexCount(0), indexCount(0), pModel(nullptr), pIndices(nullptr)
{
	//Load the model data
	if(!LoadModel(modelfile))
	{
		throw std::exception(("Failed to load model " + modelfile).c_str());
	}

	//Initialize the vertex and the index buffer
	if (!InitializeBuffers(_direct3D->GetDevice()))
	{
		throw std::exception("Failed to load vertex and index Buffer");
	}
}

Hydro::Model::~Model()
{
	//Release Buffers
	ShutdownBuffers();

	//Release Model
	ReleaseModel();
}

bool Hydro::Model::Update(float eTime)
{
	return true;
}

bool Hydro::Model::Draw(ID3D11DeviceContext* deviceContext, float eTime)
{
	unsigned int stride;
	unsigned int offset;

	//Set the vertex stride and offset
	stride = sizeof(ModelType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return true;
}

int Hydro::Model::GetIndexCount()
{
	return indexCount;
}

bool Hydro::Model::InitializeBuffers(ID3D11Device* device)
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	/*vertices = new VertexType[vertexCount];
	if (!vertices)
	{
		return false;
	}
	
	for (unsigned int i = 0; i < vertexCount; i++)
	{
		vertices[i].position = pModel->position;
		vertices[i].color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	}*/

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(ModelType) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	//Give the subresource structure a pointer to the vertex data
	vertexData.pSysMem = pModel;
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
	indexData.pSysMem = pIndices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &pIndexBuffer);
	if (FAILED(result))
	{
		return false;
	}
	return true;
}

void Hydro::Model::ShutdownBuffers()
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

bool Hydro::Model::LoadModel(const std::string& path)
{
	std::string text = "";
	unsigned short int length = 0;
	bool result = true;
	try
	{
		BinaryFile file(path, BinaryFile::Option::Load);

		do
		{
			file.LoadData<unsigned short int>(&length);
			file.GetString(&text, length);
			if (text == "Vertex Count:")
			{
				file.LoadData<unsigned int>(&vertexCount);
				pModel = new ModelType[vertexCount];
				if (!pModel)
				{
					return false;
				}
			}
			else if (text == "Face Count:")
			{
				file.LoadData<unsigned int>(&indexCount);
				indexCount = indexCount * 3;
				pIndices = new unsigned int[indexCount];
				if (!pIndices)
				{
					return false;
				}
			}
			else if (text == "Vertices:")
			{
				for (unsigned int i = 0; i < vertexCount; i++)
				{
					file.LoadData<float>(&(pModel[i].position.x));
					file.LoadData<float>(&(pModel[i].position.y));
					file.LoadData<float>(&(pModel[i].position.z));
					pModel[i].color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
				}
			}
			else if (text == "Faces:")
			{
				for (unsigned int i = 0; i < indexCount; i++)
				{
					file.LoadData<unsigned int>(&(pIndices[i]));
					pIndices[i] -= 1; //Convert to 0-Based counting 
				}
			}
		} while (!file.Error() && file.TillEnd() > 0);

	}
	catch (std::exception)
	{
		return false;
	}
	return true;
}

void Hydro::Model::ReleaseModel()
{
	if (pModel)
	{
		delete[] pModel;
		pModel = nullptr;
	}

	if (pIndices)
	{
		delete[] pIndices;
		pIndices = nullptr;
	}
}
