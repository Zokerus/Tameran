#ifndef SPRITE
#define SPRITE

//Includes
#include <d3d11.h>
#include <DirectXMath.h>
#include <string>

//My Includes
#include "Texture.h"
#include "Forms/Rectangle.h"

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
		Sprite(ID3D11Device* device, ID3D11DeviceContext* _deviceContext, int _screenWidth, int _screenHeight, std::string _textureFilename, Rectangle& _srcRect);
		Sprite(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int screenWidth, int screenHeight, std::string textureFilename);
		Sprite(ID3D11Device* device, int screenWidth, int screenHeight, Texture* texture, int imageWidth, int imageHeight);
		Sprite(ID3D11Device* device, int screenWidth, int screenHeight, Texture* texture);
		~Sprite();

		bool Update(ID3D11DeviceContext* _deviceContext, DirectX::XMINT2& _pos);
		bool Update(ID3D11DeviceContext* _deviceContext, Rectangle& _dstRect);
		bool Render(ID3D11DeviceContext* deviceContext);

		void SetTexture(Texture* texture);

		int GetIndexCount() const;
		ID3D11ShaderResourceView* GetTexture() const;
		int GetImageWidth() const;
		int GetImageHeight() const;

	private:
		//void InitializeParameters(int screenWidth, int screenHeight, int imageWidth, int imageHeight);
		bool InitializeBuffers(ID3D11Device* device);
		void ShutdownBuffers();

		bool LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::string textureFilename, Rectangle& _srcRect);
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
		Rectangle prevRect;
	};
}
#endif // !SPRITE
