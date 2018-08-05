#ifndef SHADERMANAGER
#define SHADERMANAGER

//My Includes
#include "../Direct3D.h"
#include "TextureShader.h"
#include "FontShader.h"
#include "ColorShader.h"

//////////////////////////////////////////////////////////////////////////
//Class: ShaderManager
//////////////////////////////////////////////////////////////////////////
namespace Hydro
{
	class ShaderManager
	{
	public:
		ShaderManager(ID3D11Device* device, HWND hWnd);

		bool RenderTextureShader(ID3D11DeviceContext *deviceContext, int indexCount, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX projection, ID3D11ShaderResourceView *texture);
		bool RenderFontShader(ID3D11DeviceContext *deviceContext, int indexCount, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX ortho, ID3D11ShaderResourceView *texture, DirectX::XMVECTORF32 color);
		bool RenderColorShader(ID3D11DeviceContext* deviceContext, int indexCount, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX projection);

	private:
		TextureShader textureShader;
		FontShader fontShader;
		ColorShader colorShader;
	};
}

#endif // !SHADERMANAGER


