#ifndef SPRITE
#define SPRITE

//Includes
#include <d3d11.h>
#include <DirectXMath.h>
#include <string>

//My Includes
#include "Texture.h"

//////////////////////////////////////////////////////////////////////////
//Class: Sprite
//////////////////////////////////////////////////////////////////////////
namespace Hydro
{
	class Sprite
	{
	private:
		struct VertexType
		{
			DirectX::XMFLOAT3 position;
			DirectX::XMFLOAT2 texture;
		};

	public:
		Sprite(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int screenWidth, int screenHeight, std::string textureFilename, int imageWidth, int imageHeight);
		Sprite(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int screenWidth, int screenHeight, std::string textureFilename);
		Sprite(ID3D11Device* device, int screenWidth, int screenHeight, Texture* texture, int imageWidth, int imageHeight);
		Sprite(ID3D11Device* device, int screenWidth, int screenHeight, Texture* texture);
		~Sprite();

		bool Update(ID3D11DeviceContext* deviceContext, int xPosition, int yPosition);
		bool Render(ID3D11DeviceContext* deviceContext);

		void SetTexture(Texture* texture);

		int GetIndexCount() const;
		ID3D11ShaderResourceView* GetTexture() const;
		int GetImageWidth() const;
		int GetImageHeight() const;

	private:
		void InitializeParameters(int screenWidth, int screenHeight, int imageWidth, int imageHeight);
		bool InitializeBuffers(ID3D11Device* device);
		void ShutdownBuffers();

		bool LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::string textureFilename);
		void ReleaseTexture();

	private:
		ID3D11Buffer* pVertexBuffer;
		ID3D11Buffer* pIndexBuffer;
		int vertexCount, indexCount;
		Texture* pTexture;
		bool ownTexture;
		int screenWidth, screenHeight;
		int imageWidth, imageHeight;
		int xPrevPosition, yPrevPosition;
	};
}
#endif // !SPRITE
