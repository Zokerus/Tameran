#include "Direct3D.h"
#include "../IGameWindow.h"
#include "DXErr.h"

#include <array>

Hydro::Direct3D::Direct3D(IGameWindow* window)
	: pWindow(window), pSwapChain(nullptr), pDevice(nullptr), pDeviceContext(nullptr), pRenderTargetView(nullptr), pBackBuffer(nullptr),
	pDepthStencilState(nullptr), pDepthStencilView(nullptr), pDepthDisabledStencilState(nullptr), pAlphaEnableBlendingState(nullptr), pAlphaDisableBlendingState(nullptr), pRasterStateWire(nullptr), pRasterStateSolid(nullptr), pRasterNoCullingSolid(nullptr),
	pDebug(nullptr)
{
	try
	{
		//Create Swapchain and Device
		CreateSwapChainAndDevice();

		//Create RenderTargetView
		CreateRenderTargetView();

		//Create Depth buffer
		CreateDepthBuffer();

		//Create Depth stencil
		CreateDepthStencilState();

		//Create depth stencil view
		CreateDepthStencilView();

		//Create blend states
		// alpha blending on/off
		CreateBlendState();

		//Create RasterizerState
		//solid state culling, solid no culling and wireframe no culling
		CreateRasterizerState();

		// Bind the render target view and depth stencil buffer to the output render pipeline.
		pDeviceContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), pDepthStencilView.Get());

		//create viewport
		CreateViewport();

		//Create world, projection and ortho matrices
		CreateMatrices();
	}
	catch (const DXException& e)
	{
		MessageBoxW(nullptr, e.GetErrorDescription().c_str(), e.GetErrorName().c_str(), MB_OK);
		throw std::exception("Error in creating Direct3D");
	}
}

Hydro::Direct3D::~Direct3D()
{
	//Clear the state of the device context before releasing it
	if (pDeviceContext)
	{
		pDeviceContext->ClearState();
	}

	//Release all used and created pointers of the class safely
	if (pRasterNoCullingSolid)
	{
		pRasterNoCullingSolid.Reset();
	}

	if (pRasterStateSolid)
	{
		pRasterStateSolid.Reset();
	}

	if (pRasterStateWire)
	{
		pRasterStateWire.Reset();
	}

	if (pAlphaDisableBlendingState)
	{
		pAlphaDisableBlendingState.Reset();
	}

	if (pAlphaEnableBlendingState)
	{
		pAlphaEnableBlendingState.Reset();
	}

	if (pDepthDisabledStencilState)
	{
		pDepthDisabledStencilState.Reset();
	}

	if (pDepthStencilView)
	{
		pDepthStencilView.Reset();
	}

	if (pDepthStencilState)
	{
		pDepthStencilState.Reset();
	}

	if (pBackBuffer)
	{
		pBackBuffer.Reset();
	}

	if (pRenderTargetView)
	{
		pRenderTargetView.Reset();
	}

	//pDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	if (pDebug)
	{
		pDebug.Reset();
	}

	if (pDeviceContext)
	{
		pDeviceContext.Reset();
	}

	if (pDevice)
	{
		pDevice.Reset();
	}

	if (pSwapChain)
	{
		pSwapChain.Reset();
	}
}

void Hydro::Direct3D::BeginFrame(DirectX::XMVECTORF32 color)
{
	// Clear the back buffer.
	pDeviceContext->ClearRenderTargetView(pRenderTargetView.Get(), color);

	// Clear the depth buffer.
	pDeviceContext->ClearDepthStencilView(pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

bool Hydro::Direct3D::EndFrame()
{
	// Present the back buffer to the screen since rendering is complete.
	if (pWindow->VSync())
	{
		// Lock to screen refresh rate.
		if (FAILED(pSwapChain->Present(1, 0)))
		{
			return false;
		}
	}
	else
	{
		// Present as fast as possible.
		if (FAILED(pSwapChain->Present(0, 0)))
		{
			return false;
		}
	}

	return true;
}

void Hydro::Direct3D::SetFullScreen(const bool fullscreen)
{
	pSwapChain->SetFullscreenState(fullscreen, NULL);
}

ID3D11Device* Hydro::Direct3D::GetDevice() const
{
	return pDevice.Get();
}

ID3D11DeviceContext* Hydro::Direct3D::GetDeviceContext() const
{
	return pDeviceContext.Get();
}

void Hydro::Direct3D::GetProjectionMatrix(DirectX::XMMATRIX& ProjectionMatrix)
{
	ProjectionMatrix = projectionMatrix;
}

void Hydro::Direct3D::GetWorldMatrix(DirectX::XMMATRIX& WorldMatrix)
{
	WorldMatrix = worldMatrix;
}

void Hydro::Direct3D::GetOrthoMatrix(DirectX::XMMATRIX& OrthoMatrix)
{
	OrthoMatrix = orthoMatrix;
}

void Hydro::Direct3D::GetVideoCardInfo(char* cardName, int& memory)
{
}

void Hydro::Direct3D::TurnZBufferOn()
{
	pDeviceContext->OMSetDepthStencilState(pDepthStencilState.Get(), 1);
}

void Hydro::Direct3D::TurnZBufferOff()
{
	pDeviceContext->OMSetDepthStencilState(pDepthDisabledStencilState.Get(), 1);
}

void Hydro::Direct3D::TurnOnCulling()
{
	//Set the culling rastersizer state
	pDeviceContext->RSSetState(pRasterStateSolid.Get());
}

void Hydro::Direct3D::TurnOffCulling()
{
	//Set the no back culling raster state
	pDeviceContext->RSSetState(pRasterNoCullingSolid.Get());
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
	pDeviceContext->OMSetBlendState(pAlphaEnableBlendingState.Get(), blendFactor, 0xffffffff);
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
	pDeviceContext->OMSetBlendState(pAlphaDisableBlendingState.Get(), blendFactor, 0xffffffff);
}

void Hydro::Direct3D::EnableWireframe()
{
	pDeviceContext->RSSetState(pRasterStateWire.Get());
}

void Hydro::Direct3D::DisableWireframe()
{
	pDeviceContext->RSSetState(pRasterStateSolid.Get());
}

void Hydro::Direct3D::CreateSwapChainAndDevice()
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

	// Initialize the swap chain description.
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	// Set to a double back buffer.
	swapChainDesc.BufferCount = 1;
	// Set the width and height of the back buffer.
	swapChainDesc.BufferDesc.Width = pWindow->GetWidth();
	swapChainDesc.BufferDesc.Height = pWindow->GetHeight();
	// Set regular 32-bit surface for the back buffer.
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 1;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 60;
	// Set the usage of the back buffer.
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	// Set the handle for the window to render to.
	swapChainDesc.OutputWindow = pWindow->GetHandle();
	// Discard the back buffer contents after presenting.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Windowed = true;
	// Turn multisampling off.
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	// Don't set the advanced flags.
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;


	//Create swapchain, device, device context and featurelevel and so on
	for (UINT i = 0; i < 1; i++)
	{
		result = D3D11CreateDeviceAndSwapChain(NULL, driverTypes[i], NULL, createDeviceFlags, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &swapChainDesc, &pSwapChain, &pDevice, &featureLevel, &pDeviceContext);
		if (SUCCEEDED(result))
		{
			driverType = driverTypes[i];
			break;
		}
	}

	if (FAILED(result))
	{
		OutputDebugString("Failed to create device and swapchain");
		//throw std::exception("Failed to create device and swapchain");
		throw DXException(result);
	}

	result = pDevice->QueryInterface(__uuidof(ID3D11Debug), (void**)&pDebug);
	if (FAILED(result))
	{
		OutputDebugString("Failed to create debug object");
		//throw std::exception("Failed to create debug object");
		throw DXException(result);
	}
}

void Hydro::Direct3D::CreateRenderTargetView()
{
	HRESULT result;

	//Create render target view with the help of the backbuffer
	ID3D11Texture2D* backBufferPtr = nullptr;
	result = pSwapChain->GetBuffer(NULL, __uuidof(ID3D11Texture2D), (LPVOID*)(&backBufferPtr));
	if (FAILED(result))
	{
		OutputDebugString("Failed to create backBuffer");
		//throw std::exception("Failed to create backbuffer");
		throw DXException(result);
	}

	//Create RenderTargetView
	result = pDevice->CreateRenderTargetView(backBufferPtr, nullptr, pRenderTargetView.GetAddressOf());
	if (FAILED(result))
	{
		OutputDebugString("Failed to create renderTargetView");
		//throw std::exception("Failed to create renderTargetView");
		throw DXException(result);
	}

	//Release the backbuffer pointer because it is not needed anymore
	if (backBufferPtr)
	{
		backBufferPtr->Release();
		backBufferPtr = nullptr;
	}
}

void Hydro::Direct3D::CreateDepthBuffer()
{
	HRESULT result;
	D3D11_TEXTURE2D_DESC depthBufferDesc;

	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = pWindow->GetWidth();
	depthBufferDesc.Height = pWindow->GetHeight();
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
	result = pDevice->CreateTexture2D(&depthBufferDesc, NULL, &pBackBuffer);
	if (FAILED(result))
	{
		OutputDebugString("Failed to create depth buffer texture");
		//throw std::exception("Failed to create depth buffer texture");
		throw DXException(result);
	}
}

void Hydro::Direct3D::CreateDepthStencilState()
{
	HRESULT result;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	
	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
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
	result = pDevice->CreateDepthStencilState(&depthStencilDesc, &pDepthStencilState);
	if (FAILED(result))
	{
		OutputDebugString("Failed to create depth stencil state (enabled)");
		//throw std::exception("Failed to create depth stencil state (enabled)");
		throw DXException(result);
	}

	//disable the depth feature
	depthStencilDesc.DepthEnable = false;

	// Now create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is 
	// that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
	result = pDevice->CreateDepthStencilState(&depthStencilDesc, &pDepthDisabledStencilState);
	if (FAILED(result))
	{
		OutputDebugString("Failed to create depth stencil state (disabled)");
		//throw std::exception("Failed to create depth stencil state (disbaled)");
		throw DXException(result);
	}

	// Set the depth stencil state.
	pDeviceContext->OMSetDepthStencilState(pDepthStencilState.Get(), 1);
}

void Hydro::Direct3D::CreateDepthStencilView()
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
	result = pDevice->CreateDepthStencilView(pBackBuffer.Get(), &depthStencilViewDesc, &pDepthStencilView);
	if (FAILED(result))
	{
		OutputDebugString("Failed to create depth stencil view");
		//throw std::exception("Failed to create depth stencil view");
		throw DXException(result);
	}

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	pDeviceContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), pDepthStencilView.Get());
}

void Hydro::Direct3D::CreateBlendState()
{
	HRESULT result;
	D3D11_BLEND_DESC blendStateDesc;

	// Clear the blend state description.
	ZeroMemory(&blendStateDesc, sizeof(D3D11_BLEND_DESC));

	// Create an alpha enabled blend state description.
	blendStateDesc.RenderTarget[0].BlendEnable = false;
	blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// Create the blend state using the description.
	result = pDevice->CreateBlendState(&blendStateDesc, &pAlphaDisableBlendingState);
	if (FAILED(result))
	{
		OutputDebugString("Failed to create blend state (disabled)");
		//throw std::exception("Failed to create blend state (disabled)");
		throw DXException(result);
	}

	//Switch blending on
	blendStateDesc.RenderTarget[0].BlendEnable = true;

	// Create the blend state using the description.
	result = pDevice->CreateBlendState(&blendStateDesc, &pAlphaEnableBlendingState);
	if (FAILED(result))
	{
		OutputDebugString("Failed to create blend state (enabled)");
		//throw std::exception("Failed to create blend state (enabled)");
		throw DXException(result);
	}
}

void Hydro::Direct3D::CreateRasterizerState()
{
	HRESULT result;
	D3D11_RASTERIZER_DESC rasterDesc;

	//Initialize the rasterizer description and zero memory
	ZeroMemory(&rasterDesc, sizeof(rasterDesc));

	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	result = pDevice->CreateRasterizerState(&rasterDesc, &pRasterNoCullingSolid);
	if (FAILED(result))
	{
		OutputDebugString("Failed to create rasterizer state (no culling)");
		//throw std::exception("Failed to create rasterizer state (no culling)");
		throw DXException(result);
	}

	//Solid with culling
	rasterDesc.CullMode = D3D11_CULL_BACK;

	// Create the rasterizer state from the description we just filled out.
	result = pDevice->CreateRasterizerState(&rasterDesc, &pRasterStateSolid);
	if (FAILED(result))
	{
		OutputDebugString("Failed to create rasterizer state (back culling)");
		//throw std::exception("Failed to create rasterizer state (back culling)");
		throw DXException(result);
	}

	//wireframe without culling
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;

	// Create the rasterizer state from the description we just filled out.
	result = pDevice->CreateRasterizerState(&rasterDesc, &pRasterStateWire);
	if (FAILED(result))
	{
		OutputDebugString("Failed to create rasterizer state (wireframe)");
		//throw std::exception("Failed to create rasterizer state (wireframe)");
		throw DXException(result);
	}
}

void Hydro::Direct3D::CreateViewport()
{
	//Viewport creation
	viewPort.Width = static_cast<float>(pWindow->GetWidth());
	viewPort.Height = static_cast<float>(pWindow->GetHeight());
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	// Bind the viewport.
	pDeviceContext->RSSetViewports(1, &viewPort);
}

void Hydro::Direct3D::CreateMatrices()
{
	float fieldOfView(0.0f), screenAspect(0.0f);
	float screenNear(0.1f), screenDepth(1000.0f);

	// Setup the projection matrix.
	fieldOfView = DirectX::XM_PIDIV4;
	screenAspect = (float)pWindow->GetWidth() / (float)pWindow->GetHeight();

	// Create the projection matrix for 3D rendering.
	projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);

	// Initialize the world matrix to the identity matrix.
	worldMatrix = DirectX::XMMatrixIdentity();

	// Create an orthographic projection matrix for 2D rendering.
	orthoMatrix = DirectX::XMMatrixOrthographicLH((float)pWindow->GetWidth(), (float)pWindow->GetHeight(), screenNear, screenDepth);
}

Hydro::Direct3D::DXException::DXException(HRESULT hr)
	: hr(hr)
{}

std::wstring Hydro::Direct3D::DXException::GetErrorName() const
{
	return DXGetErrorString(hr);
}

std::wstring  Hydro::Direct3D::DXException::GetErrorDescription() const
{
	std::array<wchar_t, 512> wideDescription;
	DXGetErrorDescription(hr, wideDescription.data(), wideDescription.size());
	return wideDescription.data();
}