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
		Sprite();
		~Sprite();

		bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int screenWidth, int screenHeight, std::string textureFilename, int imageWidth, int imageHeight);
		bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int screenWidth, int screenHeight, std::string textureFilename);
		bool Initialize(ID3D11Device* device, int screenWidth, int screenHeight, Texture *texture, int imageWidth, int imageHeight);
		bool Initialize(ID3D11Device* device, int screenWidth, int screenHeight, Texture *texture);
		void Shutdown();

		bool Update(ID3D11DeviceContext* deviceContext, int xPosition, int yPosition);
		bool Render(ID3D11DeviceContext* deviceContext);

		void SetTexture(Texture* texture);

		int GetIndexCount() const;
		ID3D11ShaderResourceView* GetTexture() const;
		int GetImageWidth() const;
		int GetImageHeight() const;

	private:
		void InitializeParameters(int screenWidth, int screenHeight, int imageWidth, int imageHeight);
		bool InitializeBuffers(ID3D11Device *device);
		void ShutdownBuffers();

		bool LoadTexture(ID3D11Device *device, ID3D11DeviceContext *deviceContext, std::string textureFilename);
		void ReleaseTexture();

	private:
		ID3D11Buffer* m_vertexBuffer;
		ID3D11Buffer* m_indexBuffer;
		int m_vertexCount, m_indexCount;
		Texture* m_texture;
		bool m_ownTexture;
		int m_screenWidth, m_screenHeight;
		int m_imageWidth, m_imageHeight;
		int m_xPrevPosition, m_yPrevPosition;
		bool m_ready;
	};
}
#endif // !SPRITE
