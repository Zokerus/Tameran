#ifndef TERRAIN
#define TERRAIN

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
	class Terrain
	{
	private:
		struct VertexType
		{
			DirectX::XMFLOAT3 position;
			DirectX::XMFLOAT4 color;
		};

	public:
		Terrain(Direct3D *_direct3D);
		~Terrain();

		bool Update(float eTime);
		bool Draw(ID3D11DeviceContext *deviceContext, float eTime);

		unsigned int GetIndexCount();

	private:
		bool InitializeBuffers(ID3D11Device *device);
		void ShutdownBuffers();

	private:
		ID3D11Buffer * pVertexBuffer;
		ID3D11Buffer *pIndexBuffer;
		unsigned int vertexCount, indexCount;
		int terrainDimensions = 100;
	};
}

#endif // !TERRAIN

