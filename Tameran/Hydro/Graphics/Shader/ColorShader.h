#ifndef COLORSHADER
#define COLORSHADER

//My Includes
#include "IShader.h"

namespace Hydro
{
	class ColorShader : public IShader
	{
	public:
		ColorShader(ID3D11Device* device, HWND hWnd);
		~ColorShader();

		bool Render(ID3D11DeviceContext* deviceContext, int indexCount, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX projection);

	private:
		bool SetShaderParameters(ID3D11DeviceContext *deviceContext, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX projection);
	};
}

#endif // !COLORSHADER

