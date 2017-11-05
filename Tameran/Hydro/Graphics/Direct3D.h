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

//////////////////////////////////////////////////////////////////////////
//Class: Direct3D
//////////////////////////////////////////////////////////////////////////
namespace Hydro
{
	class Direct3D
	{
	public:
		Direct3D(class IGameWindow* settings);
		Direct3D(const Direct3D& other) = delete;
		Direct3D& operator=(const Direct3D& other) = delete;
		~Direct3D();

		bool Initialize();
		void Shutdown();

		void BeginFrame(DirectX::XMVECTORF32 color);
		bool EndFrame();

		void SetFullScreen(const bool fullscreen);

		ID3D11Device* GetDevice() const;
		ID3D11DeviceContext* GetDeviceContext() const;

		void GetProjectionMatrix(DirectX::XMMATRIX &projectionMatrix);
		void GetWorldMatrix(DirectX::XMMATRIX &worldMatrix);
		void GetOrthoMatrix(DirectX::XMMATRIX &orthoMatrix);

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
		class IGameWindow*			m_window;
		IDXGISwapChain*				m_swapChain;
		ID3D11Device*				m_device;
		ID3D11DeviceContext*		m_deviceContext;
		ID3D11RenderTargetView*		m_renderTargetView;

		D3D_DRIVER_TYPE				m_driverType;
		D3D_FEATURE_LEVEL			m_featureLevel;
		D3D11_VIEWPORT				m_viewport;

		DirectX::XMMATRIX			m_projectionMatrix;
		DirectX::XMMATRIX			m_worldMatrix;
		DirectX::XMMATRIX			m_orthoMatrix;

		ID3D11DepthStencilState*	m_depthDisabledStencilState;
		ID3D11BlendState*			m_alphaEnableBlendingState;
		ID3D11BlendState*			m_alphaDisableBlendingState;
		ID3D11RasterizerState*		m_rasterStateWire;
		ID3D11RasterizerState*		m_rasterStateSolid;
		ID3D11RasterizerState*		m_rasterNoCullingSolid;

		bool m_ready;
	};
}

#endif // !DIRECT3DHEADER

