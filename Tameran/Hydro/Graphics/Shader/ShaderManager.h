#ifndef SHADERMANAGER
#define SHADERMANAGER

//My Includes
#include "../Direct3D.h"
#include "TextureShader.h"
#include "FontShader.h"

//////////////////////////////////////////////////////////////////////////
//Class: ShaderManager
//////////////////////////////////////////////////////////////////////////
namespace Hydro
{
	class ShaderManager
	{
	public:
		ShaderManager();
		~ShaderManager();

		bool Initialize(ID3D11Device* device, HWND hWnd);
		void Shutdown();

		bool RenderTextureShader(ID3D11DeviceContext *deviceContext, int indexCount, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX projection, ID3D11ShaderResourceView *texture);
		bool RenderFontShader(ID3D11DeviceContext *deviceContext, int indexCount, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX ortho, ID3D11ShaderResourceView *texture, DirectX::XMVECTORF32 color);

	private:
		bool m_ready;
		TextureShader m_textureShader;
		FontShader m_fontShader;

	};
}

#endif // !SHADERMANAGER


