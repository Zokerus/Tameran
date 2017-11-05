#include "Direct3D.h"
#include "../IGameWindow.h"

Hydro::Direct3D::Direct3D(IGameWindow* window)
	: m_window(window), m_swapChain(nullptr), m_device(nullptr), m_deviceContext(nullptr), m_renderTargetView(nullptr),
	m_depthDisabledStencilState(nullptr), m_alphaEnableBlendingState(nullptr), m_alphaDisableBlendingState(nullptr), m_rasterStateWire(nullptr), m_rasterStateSolid(nullptr), m_rasterNoCullingSolid(nullptr),
	m_ready(false)
{}

Hydro::Direct3D::~Direct3D()
{
	if (m_ready)
	{
		Shutdown();
	}
}

bool Hydro::Direct3D::Initialize()
{
	HRESULT result;

	//Initialize the creation flags to create swapchain and device
	UINT createDeviceFlags = 0u;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG

	D3D_DRIVER_TYPE driverTypes[] = { D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP, D3D_DRIVER_TYPE_REFERENCE };
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0, D3D_FEATURE_LEVEL_9_3 };
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	// Initialize the swap chain description.
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	// Set to a double back buffer.
	swapChainDesc.BufferCount = 1;
	// Set the width and height of the back buffer.
	swapChainDesc.BufferDesc.Width = m_window->GetWidth();
	swapChainDesc.BufferDesc.Height = m_window->GetHeight();
	// Set regular 32-bit surface for the back buffer.
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 1;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 60;
	// Set the usage of the back buffer.
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	// Set the handle for the window to render to.
	swapChainDesc.OutputWindow = m_window->GetHandle();
	// Discard the back buffer contents after presenting.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Windowed = true;
	// Turn multisampling off.
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	// Don't set the advanced flags.
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;


	//Create swapchain, device, device context and featurelevel and so on
	for (UINT i = 0; i < numDriverTypes; i++)
	{
		result = D3D11CreateDeviceAndSwapChain(NULL, driverTypes[i], NULL, createDeviceFlags, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, &m_featureLevel, &m_deviceContext);
		if (SUCCEEDED(result))
		{
			m_driverType = driverTypes[i];
			break;
		}
	}

	if (FAILED(result))
	{
		OutputDebugString("Failed to create device and swap chain");
		return false;
	}

	//Create render target view with the help of the backbuffer
	ID3D11Texture2D* backBuffer = nullptr;
	result = m_swapChain->GetBuffer(NULL, __uuidof(ID3D11Texture2D), (LPVOID*)(&backBuffer));
	if (FAILED(result))
	{
		return false;
	}

	result = m_device->CreateRenderTargetView(backBuffer, nullptr, &m_renderTargetView);
	if (FAILED(result))
	{
		return false;
	}

	//Release the backbuffer pointer because it is not needed anymore
	if (backBuffer)
	{
		backBuffer->Release();
		backBuffer = nullptr;
	}

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, nullptr);

	//Viewport creation
	m_viewport.Width = static_cast<float>(m_window->GetWidth());
	m_viewport.Height = static_cast<float>(m_window->GetHeight());
	m_viewport.TopLeftX = 0;
	m_viewport.TopLeftY = 0;
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
	// Bind the viewport.
	m_deviceContext->RSSetViewports(1, &m_viewport);

	//Flag the class as ready
	m_ready = true;
	return true;
}

void Hydro::Direct3D::Shutdown()
{
	//Clear the state of the device context before releasing it
	if (m_deviceContext)
	{
		m_deviceContext->ClearState();
	}

	//Release all used and created pointers of the class safely
	if (m_rasterNoCullingSolid)
	{
		m_rasterNoCullingSolid->Release();
		m_rasterNoCullingSolid = nullptr;
	}
	
	if (m_rasterStateSolid)
	{
		m_rasterStateSolid->Release();
		m_rasterStateSolid = nullptr;
	}

	if (m_rasterStateWire)
	{
		m_rasterStateWire->Release();
		m_rasterStateWire = nullptr;
	}

	if (m_alphaDisableBlendingState)
	{
		m_alphaDisableBlendingState->Release();
		m_alphaDisableBlendingState = nullptr;
	}

	if (m_alphaEnableBlendingState)
	{
		m_alphaEnableBlendingState->Release();
		m_alphaEnableBlendingState = nullptr;
	}

	if (m_depthDisabledStencilState)
	{
		m_depthDisabledStencilState->Release();
		m_depthDisabledStencilState = nullptr;
	}

	if (m_renderTargetView)
	{
		m_renderTargetView->Release();
		m_renderTargetView = nullptr;
	}

	if (m_deviceContext)
	{
		m_deviceContext->Release();
		m_deviceContext = nullptr;
	}

	if (m_device)
	{
		m_device->Release();
		m_device = nullptr;
	}

	if (m_swapChain)
	{
		m_swapChain->Release();
		m_swapChain = nullptr;
	}

	//Set the ready flag of the class to false
	m_ready = false;
}

void Hydro::Direct3D::BeginFrame(DirectX::XMVECTORF32 color)
{
	//Check if the class is ready to draw
	if (m_ready)
	{
		// Clear the back buffer.
		m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);
	}
}

bool Hydro::Direct3D::EndFrame()
{
	if (!m_ready)
	{
		//If the class is nor ready, return with a false and quit the app
		return false;
	}
	// Present the back buffer to the screen since rendering is complete.
	if (m_window->VSync())
	{
		// Lock to screen refresh rate.
		if (FAILED(m_swapChain->Present(1, 0)))
		{
			return false;
		}
	}
	else
	{
		// Present as fast as possible.
		if (FAILED(m_swapChain->Present(0, 0)))
		{
			return false;
		}
	}

	return true;
}

void Hydro::Direct3D::SetFullScreen(const bool fullscreen)
{
	m_swapChain->SetFullscreenState(fullscreen, NULL);
}

ID3D11Device* Hydro::Direct3D::GetDevice()
{
	return m_device;
}

ID3D11DeviceContext* Hydro::Direct3D::GetDeviceContext()
{
	return m_deviceContext;
}

void Hydro::Direct3D::GetProjectionMatrix(DirectX::XMMATRIX & projectionMatrix)
{
}

void Hydro::Direct3D::GetWorldMatrix(DirectX::XMMATRIX & worldMatrix)
{
}

void Hydro::Direct3D::GetOrthoMatrix(DirectX::XMMATRIX & orthoMatrix)
{
}

void Hydro::Direct3D::GetVideoCardInfo(char * cardName, int & memory)
{
}

void Hydro::Direct3D::TurnZBufferOn()
{
}

void Hydro::Direct3D::TurnZBufferOff()
{
}

void Hydro::Direct3D::TurnOnCulling()
{
}

void Hydro::Direct3D::TurnOffCulling()
{
}

void Hydro::Direct3D::TurnAlphaBlendingOn()
{
}

void Hydro::Direct3D::TurnAlphaBlendingOff()
{
}

void Hydro::Direct3D::EnableWireframe()
{
}

void Hydro::Direct3D::DisableWireframe()
{
}
