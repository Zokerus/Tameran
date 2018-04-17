#ifndef FONTSHADER
#define FONTSHADER

//My Includes
#include "IShader.h"

namespace Hydro
{
	class FontShader : public IShader
	{
	private:
		struct PixelBufferType
		{
			DirectX::XMVECTORF32 pixelColor;
		};

	public:
		FontShader(ID3D11Device* device, HWND hWnd);
		~FontShader();

		bool Render(ID3D11DeviceContext *deviceContext, int indexCount, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX projection, ID3D11ShaderResourceView *texture, DirectX::XMVECTORF32 pixelColor);

	private:
		bool SetShaderParameters(ID3D11DeviceContext *deviceContext, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX projection, ID3D11ShaderResourceView *texture, DirectX::XMVECTORF32 pixelColor);
	
	private:
		ID3D11SamplerState *m_sampleState;
		ID3D11Buffer* m_pixelBuffer;
	};
}

#endif // !FONTSHADER