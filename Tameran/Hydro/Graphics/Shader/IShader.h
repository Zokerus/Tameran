#ifndef ISHADER
#define ISHADER

//Includes
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <string>
#include <fstream>

//My Includes
#include "../../Util/Globals.h"

//////////////////////////////////////////////////////////////////////////
//Class: IShader
//////////////////////////////////////////////////////////////////////////
namespace Hydro
{
	class IShader
	{
	protected:
		struct MatrixBufferType
		{
			DirectX::XMMATRIX world;
			DirectX::XMMATRIX view;
			DirectX::XMMATRIX projection;
		};

	public:
		IShader(std::wstring shaderName);
		~IShader();

		virtual bool Render(ID3D11DeviceContext *deviceContext, int indexCount, MatrixBufferType matrices);

	protected:
		virtual bool SetShaderParameters(ID3D11DeviceContext *deviceContext, MatrixBufferType *matrices);
		void OutputShaderErrorMessage(ID3DBlob *errorMessage, HWND hwnd, std::string shaderFilename);

	protected:
		ID3D11VertexShader* pVertexShader;
		ID3D11PixelShader* pPixelShader;
		ID3D11InputLayout* pLayout;
		ID3D11Buffer* matrixBuffer;
		std::wstring shaderName;
	};
}

#endif // !ISHADER

