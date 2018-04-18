#ifndef DIRECT3DHEADER
#define DIRECT3DHEADER

//linking
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

//Includes
#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <string>
#include <wrl.h>

//////////////////////////////////////////////////////////////////////////
//Class: Direct3D
//////////////////////////////////////////////////////////////////////////
namespace Hydro
{
	class Direct3D
	{
		class DXException : public std::exception
		{
		public:
			DXException(HRESULT hr);
			std::wstring GetErrorName() const;
			std::wstring GetErrorDescription() const;
		private:
			HRESULT hr;
		};

	public:
		Direct3D(class IGameWindow* settings);
		Direct3D(const Direct3D& other) = delete;
		Direct3D& operator=(const Direct3D& other) = delete;
		~Direct3D();

		void BeginFrame(DirectX::XMVECTORF32 color);
		bool EndFrame();

		void SetFullScreen(const bool fullscreen);

		ID3D11Device* GetDevice() const;
		ID3D11DeviceContext* GetDeviceContext() const;

		void GetProjectionMatrix(DirectX::XMMATRIX& projectionMatrix);
		void GetWorldMatrix(DirectX::XMMATRIX& worldMatrix);
		void GetOrthoMatrix(DirectX::XMMATRIX& orthoMatrix);

		void GetVideoCardInfo(char* cardName, int& memory);

		void TurnZBufferOn();
		void TurnZBufferOff();
		void TurnOnCulling();
		void TurnOffCulling();
		void TurnAlphaBlendingOn();
		void TurnAlphaBlendingOff();
		void EnableWireframe();
		void DisableWireframe();

	private:
		void CreateSwapChainAndDevice();
		void CreateRenderTargetView();
		void CreateDepthBuffer();
		void CreateDepthStencilState();
		void CreateDepthStencilView();
		void CreateBlendState();
		void CreateRasterizerState();
		void CreateViewport();
		void CreateMatrices();

	private:
		class IGameWindow*								pWindow;
		Microsoft::WRL::ComPtr<IDXGISwapChain>			pSwapChain;
		Microsoft::WRL::ComPtr<ID3D11Device>			pDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>		pDeviceContext;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	pRenderTargetView;
		Microsoft::WRL::ComPtr<ID3D11Texture2D>			pBackBuffer;

		D3D_DRIVER_TYPE									driverType;
		D3D_FEATURE_LEVEL								featureLevel;
		D3D11_VIEWPORT									viewPort;

		DirectX::XMMATRIX								projectionMatrix;
		DirectX::XMMATRIX								worldMatrix;
		DirectX::XMMATRIX								orthoMatrix;

		Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	pDepthStencilState;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	pDepthStencilView;

		Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	pDepthDisabledStencilState;
		Microsoft::WRL::ComPtr<ID3D11BlendState>		pAlphaEnableBlendingState;
		Microsoft::WRL::ComPtr<ID3D11BlendState>		pAlphaDisableBlendingState;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState>	pRasterStateWire;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState>	pRasterStateSolid;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState>	pRasterNoCullingSolid;

		Microsoft::WRL::ComPtr<ID3D11Debug>				pDebug;
	};
}

#endif // !DIRECT3DHEADER

