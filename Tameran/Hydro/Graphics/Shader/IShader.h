#ifndef ISHADER
#define ISHADER

//Includes
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
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
		struct MatrixBufferType
		{
			DirectX::XMMATRIX world;
			DirectX::XMMATRIX view;
			DirectX::XMMATRIX projection;
		};

	public:
		IShader();
		~IShader();

		bool Initialize(ID3D11Device *device, HWND hWnd);
		void Shutdown();
		bool Render(ID3D11DeviceContext *deviceContext, int indexCount, MatrixBufferType matrices);

	protected:
		bool SetShaderParameters(ID3D11DeviceContext *deviceContext, MatrixBufferType *matrices);
		bool RenderShader(ID3D11DeviceContext *deviceContext, int indexCount);
		void OutputShaderErrorMessage(ID3DBlob *errorMessage, HWND hwnd, std::string shaderFilename);

	protected:
		ID3D11VertexShader *m_vertexShader;
		ID3D11PixelShader *m_pixelShader;
		ID3D11InputLayout *m_layout;
		ID3D11Buffer *m_matrixBuffer;
		bool m_ready;
	};
}

#endif // !ISHADER

