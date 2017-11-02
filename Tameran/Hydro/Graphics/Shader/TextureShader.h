#ifndef TEXTURESHADER
#define TEXTURESHADER

//My Includes
#include "IShader.h"

//////////////////////////////////////////////////////////////////////////
//Class: TextureShader
//////////////////////////////////////////////////////////////////////////
namespace Hydro
{
	class TextureShader : public IShader
	{
	public:
		TextureShader();
		~TextureShader();

		bool Initialize(ID3D11Device *device, HWND hWnd);
		void Shutdown();
		bool Render(ID3D11DeviceContext* deviceContext, int indexCount, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX projection, ID3D11ShaderResourceView *texture);

	private:
		bool SetShaderParameters(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX projection, ID3D11ShaderResourceView *texture);

	private:
		ID3D11SamplerState* m_sampleState;
	};
}
#endif // !TEXTURESHADER

