#include "ShaderClass.h"
#include <d3dcompiler.h>
#include <WICTextureLoader.h>

using namespace DirectX;

ShaderClass::ShaderClass()
{
}

bool ShaderClass::InitializeShaders(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dContext)
{
	// Create Vertex Shader File
	ID3DBlob *pVSBlob = NULL;
	HRESULT result = D3DReadFileToBlob(L".\\VertexShader.cso", &pVSBlob);

	// Check the result
	if (result != S_OK)
	{
		MessageBox(NULL, L"Error Loading Vertex Shader (ShaderClass.CPP)", L"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	// Add the shader to directX
	result = d3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &vertexShader);

	// Check if it was not successful
	if (result != S_OK)
	{
		pVSBlob->Release();
		MessageBox(NULL, L"Error adding Vertex Shader to Direct X (ShaderClass.CPP)", L"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	// Define how vertices are structered
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	// Define number of elements in the vertex structure
	UINT numElements = ARRAYSIZE(layout);

	// Create input layout for input assember based on description
	result = d3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &vertexLayout);

	pVSBlob->Release();
	// Check layout was created sucessfully
	if (result != S_OK)
	{
		MessageBox(NULL, L"Error creating input layout (ShaderClass.CPP)", L"Error", MB_OK | MB_ICONERROR);
		return false;
	}


	// Compile the pixel shader
	ID3DBlob* pPSBlob = NULL;
	result = D3DReadFileToBlob(L".\\PixelShader.cso", &pPSBlob);

	// Check pixel shader was loaded successfully
	if (result != S_OK)
	{
		MessageBox(NULL, L"Error adding Vertex Shader to Direct X (ShaderClass.CPP)", L"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	// Add the pixel shader to DirectX
	result = d3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &pixelShader);
	pPSBlob->Release();

	// Check creating pixel shader was successful
	if (result != S_OK) 
	{
		MessageBox(NULL, L"Error creating pixel shader (ShaderClass.CPP)", L"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	// Define and create the sampler object
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	//Create the sampler object from the sampler description
	result = d3dDevice->CreateSamplerState(&sampDesc, &sampler);

	if (result != S_OK)
	{
		MessageBox(NULL, L"Error Creating Sampler State (ShaderClass.CPP)", L"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	return true;
}

void ShaderClass::ShutdownShaders()
{
	if (vertexShader)
		vertexShader->Release();

	if (pixelShader)
		pixelShader->Release();

	if (vertexLayout)
		vertexLayout->Release();

	if (sampler)
		sampler->Release();

	if (tex)
		tex->Release();

	vertexShader = 0;
	pixelShader = 0;
	vertexLayout = 0;
	sampler = 0;
	tex = 0;
}

void ShaderClass::RenderShaders(ID3D11DeviceContext *d3dContext)
{
	// Set the input layout for input assembler
	d3dContext->IASetInputLayout(vertexLayout);

	// Render a triangle.  Not directly from vertices but from applying the shaders
	d3dContext->VSSetShader(vertexShader, NULL, 0);
	d3dContext->PSSetShader(pixelShader, NULL, 0);

	// Set the sampler to use to affect pixels selected from texture
	d3dContext->PSSetSamplers(0, 1, &sampler);

	return;
}

