#include "Model.h"
#include "../../Util/BinaryFile.h"

Hydro::Model::Model(Direct3D *_direct3D, const std::string modelfile, DirectX::XMVECTORF32 color)
	: pVertexBuffer(nullptr), pIndexBuffer(nullptr), vertexCount(0), indexCount(0), pModel(nullptr), pIndices(nullptr)
{
	bool result = false;

	//Load the model data
	result = LoadModel(modelfile);
	if (!result)
	{
		throw std::exception(("Failed to load model " + modelfile).c_str());
	}
}

Hydro::Model::~Model()
{
}

bool Hydro::Model::LoadModel(const std::string& path)
{
	std::string test = "";
	unsigned char length = 0;
	BinaryFile file(path, BinaryFile::Option::Load);
	bool result = file.LoadData<unsigned char>(&length);
	file.GetString(&test, 14);



	return true;
}
