#include "DirectXClass.h"

DirectXClass::DirectXClass()
{
}

bool DirectXClass::InitializeDirectX(HWND windowHandle)
{
	// Get the current size of the window and store the height and width
	RECT rectDimensions;
	GetClientRect(windowHandle, &rectDimensions);

	LONG width = rectDimensions.right - rectDimensions.left;
	LONG height = rectDimensions.bottom - rectDimensions.top;

	// List of possible feature levels
	D3D_FEATURE_LEVEL featureLevels[] = 
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0 
	};

	// Store the selected feature level
	D3D_FEATURE_LEVEL selectedFeatureLevel;

	// Create the swap chain description
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = windowHandle;
	swapChainDesc.Windowed = true;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// Create result variable to check creation status of objects
	HRESULT result;
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &d3dDevice, &selectedFeatureLevel, &d3dContext);
	
	// Check result, if it failed, throw error
	if (result != S_OK)
	{
		MessageBox(windowHandle, L"Failed to Create Device and SwapChain (DirectXClass.CPP)", L"Error", MB_OK | MB_ICONERROR);
		return false;	
	}

	// Create the back buffer texture
	ID3D11Texture2D *backBufferTexture;
	result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *)&backBufferTexture);

	// Check result of back buffer
	if (result != S_OK) 
	{
		MessageBox(windowHandle, L"Failed to create BackBuffer Texture (DirectXClass.CPP)", L"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	// Create the d3d back buffer using the texture created above
	result = d3dDevice->CreateRenderTargetView(backBufferTexture, 0, &d3dBackBuffer);

	// If backBuffer texture is being used, release it to save memory (no longer needed)
	if (backBufferTexture != NULL)
		backBufferTexture->Release();

	// Check result
	if (result != S_OK) 
	{
		MessageBox(windowHandle, L"Failed to create RenderTargetView (DirectXClass.CPP)", L"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	// DepthStencil Description
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_TEXTURE2D_DESC));

	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	// Create texture for depth Buffer
	d3dDevice->CreateTexture2D(&depthStencilDesc, NULL, &depthStencilBuffer);

	// Create depthStencil object based on texture
	d3dDevice->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView);

	// bind the back buffer to the depth stencil
	d3dContext->OMSetRenderTargets(1, &d3dBackBuffer, depthStencilView);

	// Blend State Description (for transparency)
	D3D11_BLEND_DESC blendStateDesc;
	ZeroMemory(&blendStateDesc, sizeof(blendStateDesc));
	blendStateDesc.RenderTarget[0].BlendEnable = true;
	blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	result = d3dDevice->CreateBlendState(&blendStateDesc, &d3dBlendState);
	if (result != S_OK)
	{
		MessageBox(windowHandle, L"Failed to create blend State (DirectXClass.CPP)", L"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	d3dContext->OMSetBlendState(d3dBlendState, 0, 0xffffffff);

	// Set up the viewport
	D3D11_VIEWPORT viewport;
	viewport.Width = static_cast<float>(width);
	viewport.Height = static_cast<float>(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	d3dContext->RSSetViewports(1, &viewport);

	return true;
}

void DirectXClass::ShutdownDirectX()
{
	if (d3dDevice)
		d3dDevice->Release();

	if (d3dContext)
		d3dContext->Release();

	if (d3dBackBuffer)
		d3dBackBuffer->Release();

	if (depthStencilView != NULL)
		depthStencilView->Release();

	if (depthStencilBuffer != NULL)
		depthStencilBuffer->Release();
}