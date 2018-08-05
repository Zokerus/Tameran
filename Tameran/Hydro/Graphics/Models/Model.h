#ifndef MODEL
#define MODEL

//Includes
#include <d3d11.h>
#include <DirectXMath.h>
#include <string>

//My Includes
#include "../Direct3D.h"

//////////////////////////////////////////////////////////////////////////
//Class: Model (Static)
//////////////////////////////////////////////////////////////////////////
namespace Hydro
{
	class Model
	{
	private:
		struct VertexType
		{
			DirectX::XMFLOAT3 position;
			DirectX::XMFLOAT4 color;
		};

		struct ModelType
		{
			DirectX::XMFLOAT3 position;
			DirectX::XMFLOAT4 color;
		};

	public:
		Model(Direct3D *_direct3D, const std::string modelfile);
		~Model();

		bool Update(float eTime);
		bool Draw(ID3D11DeviceContext *deviceContext, float eTime);

		int GetIndexCount();

	private:
		bool InitializeBuffers(ID3D11Device *device);
		void ShutdownBuffers();

		bool LoadModel(const std::string& path);
		void ReleaseModel();

	private:
		ID3D11Buffer *pVertexBuffer;
		ID3D11Buffer *pIndexBuffer;
		unsigned int vertexCount, indexCount;
		ModelType *pModel;
		unsigned int *pIndices;
	};
}
#endif // !MODEL

