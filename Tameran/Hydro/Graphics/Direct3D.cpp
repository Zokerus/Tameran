#include "Direct3D.h"
#include "../IGameWindow.h"

Hydro::Direct3D::Direct3D(IGameWindow* window)
	: m_window(window), m_swapChain(nullptr), m_device(nullptr), m_deviceContext(nullptr), m_renderTargetView(nullptr), m_backBuffer(nullptr),
	m_depthStencilState(nullptr), m_depthStencilView(nullptr), m_depthDisabledStencilState(nullptr), m_alphaEnableBlendingState(nullptr), m_alphaDisableBlendingState(nullptr), m_rasterStateWire(nullptr), m_rasterStateSolid(nullptr), m_rasterNoCullingSolid(nullptr),
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
	//Create Swapchain and Device
	if (!CreateSwapChainAndDevice())
	{
		return false;
	}

	//Create RenderTargetView
	if (!CreateRenderTargetView())
	{
		return false;
	}

	//Create Depth buffer
	if (!CreateDepthBuffer())
	{
		return false;
	}

	//Create Depth stecil
	if (!CreateDepthStencilState(m_depthStencilState, true))
	{
		return false;
	}

	//Create depth stencil view
	if (!CreateDepthStencilView(m_depthStencilState, m_depthStencilView))
	{
		return false;
	}

	// Now create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is 
	// that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
	if (!CreateDepthStencilState(m_depthDisabledStencilState, false))
	{
		return false;
	}

	//Create blend states
	// alpha blending off
	if (!CreateBlendState(m_alphaDisableBlendingState, false))
	{
		return false;
	}

	// alpha blending on
	if (!CreateBlendState(m_alphaEnableBlendingState, true))
	{
		return false;
	}

	//Create RasterizerState
	//solid state culling
	if (!CreateRasterizerState(m_rasterStateSolid, D3D11_CULL_BACK, D3D11_FILL_SOLID))
	{
		return false;
	}

	//solid state no culling
	if (!CreateRasterizerState(m_rasterStateSolid, D3D11_CULL_NONE, D3D11_FILL_SOLID))
	{
		return false;
	}

	//wireframe
	if (!CreateRasterizerState(m_rasterStateSolid, D3D11_CULL_BACK, D3D11_FILL_WIREFRAME))
	{
		return false;
	}

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	//create viewport
	if (!CreateViewport())
	{
		return false;
	}

	//Create world, projection and ortho matrices
	CreateMatrices();

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

	if (m_depthStencilView)
	{
		m_depthStencilView->Release();
		m_depthStencilView = nullptr;
	}

	if (m_depthStencilState)
	{
		m_depthStencilState->Release();
		m_depthStencilState = nullptr;
	}

	if (m_backBuffer)
	{
		m_backBuffer->Release();
		m_backBuffer = nullptr;
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

		// Clear the depth buffer.
		m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
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

ID3D11Device* Hydro::Direct3D::GetDevice() const
{
	return m_device;
}

ID3D11DeviceContext* Hydro::Direct3D::GetDeviceContext() const
{
	return m_deviceContext;
}

void Hydro::Direct3D::GetProjectionMatrix(DirectX::XMMATRIX& projectionMatrix)
{
	projectionMatrix = m_projectionMatrix;
}

void Hydro::Direct3D::GetWorldMatrix(DirectX::XMMATRIX& worldMatrix)
{
	worldMatrix = m_worldMatrix;
}

void Hydro::Direct3D::GetOrthoMatrix(DirectX::XMMATRIX& orthoMatrix)
{
	orthoMatrix = m_orthoMatrix;
}

void Hydro::Direct3D::GetVideoCardInfo(char * cardName, int & memory)
{
}

void Hydro::Direct3D::TurnZBufferOn()
{
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);
}

void Hydro::Direct3D::TurnZBufferOff()
{
	m_deviceContext->OMSetDepthStencilState(m_depthDisabledStencilState, 1);
}

void Hydro::Direct3D::TurnOnCulling()
{
	//Set the culling rastersizer state
	m_deviceContext->RSSetState(m_rasterStateSolid);
}

void Hydro::Direct3D::TurnOffCulling()
{
	//Set the no back culling raster state
	m_deviceContext->RSSetState(m_rasterNoCullingSolid);
}

void Hydro::Direct3D::TurnAlphaBlendingOn()
{
	float blendFactor[4];

	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	// Turn on the alpha blending.
	m_deviceContext->OMSetBlendState(m_alphaEnableBlendingState, blendFactor, 0xffffffff);
}

void Hydro::Direct3D::TurnAlphaBlendingOff()
{
	float blendFactor[4];

	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	// Turn off the alpha blending.
	m_deviceContext->OMSetBlendState(m_alphaDisableBlendingState, blendFactor, 0xffffffff);
}

void Hydro::Direct3D::EnableWireframe()
{
	m_deviceContext->RSSetState(m_rasterStateWire);
}

void Hydro::Direct3D::DisableWireframe()
{
	m_deviceContext->RSSetState(m_rasterStateSolid);
}

bool Hydro::Direct3D::CreateSwapChainAndDevice()
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

	return true;
}

bool Hydro::Direct3D::CreateRenderTargetView()
{
	HRESULT result;

	//Create render target view with the help of the backbuffer
	ID3D11Texture2D* backBufferPtr = nullptr;
	result = m_swapChain->GetBuffer(NULL, __uuidof(ID3D11Texture2D), (LPVOID*)(&backBufferPtr));
	if (FAILED(result))
	{
		OutputDebugString("Failed to create backBuffer");
		return false;
	}

	//Create RenderTargetView
	result = m_device->CreateRenderTargetView(backBufferPtr, nullptr, &m_renderTargetView);
	if (FAILED(result))
	{
		OutputDebugString("Failed to create renderTargetView");
		return false;
	}

	//Release the backbuffer pointer because it is not needed anymore
	if (backBufferPtr)
	{
		backBufferPtr->Release();
		backBufferPtr = nullptr;
	}

	return true;
}

bool Hydro::Direct3D::CreateDepthBuffer()
{
	HRESULT result;
	D3D11_TEXTURE2D_DESC depthBufferDesc;

	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = m_window->GetWidth();
	depthBufferDesc.Height = m_window->GetHeight();
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_backBuffer);
	if (FAILED(result))
	{
		OutputDebugString("Failed to create depth buffer texture");
		return false;
	}

	return true;
}

bool Hydro::Direct3D::CreateDepthStencilState(ID3D11DepthStencilState* stencilState, bool enable)
{
	HRESULT result;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	
	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = enable;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	result = m_device->CreateDepthStencilState(&depthStencilDesc, &stencilState);
	if (FAILED(result))
	{
		OutputDebugString("Failed to create depth stencil state");
		return false;
	}

	// Set the depth stencil state.
	m_deviceContext->OMSetDepthStencilState(stencilState, 1);

	return true;
}

bool Hydro::Direct3D::CreateDepthStencilView(ID3D11DepthStencilState * stencilState, ID3D11DepthStencilView * stencilView)
{
	HRESULT result;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

	// Initialize the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = m_device->CreateDepthStencilView(m_backBuffer, &depthStencilViewDesc, &m_depthStencilView);
	if (FAILED(result))
	{
		OutputDebugString("Failed to create depth stencil view");
		return false;
	}

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
	return true;
}

bool Hydro::Direct3D::CreateBlendState(ID3D11BlendState* blendState, bool enable)
{
	HRESULT result;
	D3D11_BLEND_DESC blendStateDesc;

	// Clear the blend state description.
	ZeroMemory(&blendStateDesc, sizeof(D3D11_BLEND_DESC));

	// Create an alpha enabled blend state description.
	blendStateDesc.RenderTarget[0].BlendEnable = enable;
	blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// Create the blend state using the description.
	result = m_device->CreateBlendState(&blendStateDesc, &blendState);
	if (FAILED(result))
	{
		OutputDebugString("Failed to create blend state");
		return false;
	}

	return true;
}

bool Hydro::Direct3D::CreateRasterizerState(ID3D11RasterizerState* rasterState, D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode)
{
	HRESULT result;
	D3D11_RASTERIZER_DESC rasterDesc;

	//Initialize the rasterizer description and zero memory
	ZeroMemory(&rasterDesc, sizeof(rasterDesc));

	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = cullMode;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = fillMode;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	result = m_device->CreateRasterizerState(&rasterDesc, &rasterState);
	if (FAILED(result))
	{
		OutputDebugString("Failed to create rasterizer state");
		return false;
	}

	return true;
}

bool Hydro::Direct3D::CreateViewport()
{
	//Viewport creation
	m_viewport.Width = static_cast<float>(m_window->GetWidth());
	m_viewport.Height = static_cast<float>(m_window->GetHeight());
	m_viewport.TopLeftX = 0;
	m_viewport.TopLeftY = 0;
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
	// Bind the viewport.
	m_deviceContext->RSSetViewports(1, &m_viewport);

	return true;
}

void Hydro::Direct3D::CreateMatrices()
{
	float fieldOfView(0.0f), screenAspect(0.0f);
	float screenNear(0.1f), screenDepth(1000.0f);

	// Setup the projection matrix.
	fieldOfView = DirectX::XM_PIDIV4;
	screenAspect = (float)m_window->GetWidth() / (float)m_window->GetHeight();

	// Create the projection matrix for 3D rendering.
	m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);

	// Initialize the world matrix to the identity matrix.
	m_worldMatrix = DirectX::XMMatrixIdentity();

	// Create an orthographic projection matrix for 2D rendering.
	m_orthoMatrix = DirectX::XMMatrixOrthographicLH((float)m_window->GetWidth(), (float)m_window->GetHeight(), screenNear, screenDepth);
}
