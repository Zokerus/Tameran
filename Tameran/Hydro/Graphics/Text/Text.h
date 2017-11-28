#ifndef TEXTHEADER
#define TEXTHEADER

//Includes
#include <DirectXColors.h>

//My Includes
#include "Font.h"
#include "../Shader/ShaderManager.h"

//////////////////////////////////////////////////////////////////////////
//Class: Text
//////////////////////////////////////////////////////////////////////////
namespace Hydro
{
	class Text
	{
	public:
		Text(int screenWidth, int screenHeight, int maxLength);
		~Text();

		bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, bool shadow, Font* font, char* text, DirectX::XMINT2 pos, DirectX::XMVECTORF32 color);
		void Shutdown();
		bool Render(ID3D11DeviceContext* deviceContext, ShaderManager* shaderManager, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX orthoMatrix, ID3D11ShaderResourceView* fontTexture);
		bool Render(ID3D11DeviceContext* deviceContext, ShaderManager* shaderManager, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX orthoMatrix, ID3D11ShaderResourceView* fontTexture, DirectX::XMVECTORF32 foreColor);
	
		bool UpdateSentence(ID3D11DeviceContext* deviceContext, Font* font, char* text, DirectX::XMINT2 pos, DirectX::XMVECTORF32 color);
	private:
		bool RenderSentence(ID3D11DeviceContext* deviceContext, ShaderManager* shaderManager, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX orthoMatrix, ID3D11ShaderResourceView* fontTexture, DirectX::XMVECTORF32 foreColor);
	
	private:
		ID3D11Buffer *m_vertexBuffer, *m_indexBuffer, *m_vertexBuffer2, *m_indexBuffer2;
		DirectX::XMVECTORF32 m_pixelColor;
		DirectX::XMINT2 m_pos;
		int m_screenWidth, m_screenHeight, m_maxLength, m_vertexCount, m_indexCount;
		bool m_shadow, m_ready;
	};
}

#endif // !TEXTHEADER

