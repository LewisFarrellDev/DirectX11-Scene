#include "LightClass.h"
#include "InputClass.h"

LightClass::LightClass()
{
}

bool LightClass::InitializeLightClass(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dContext, InputClass *input)
{
	// Store the d3d device and contecxt
	this->d3dDevice = d3dDevice;
	this->d3dContext = d3dContext;
	this->inputClass = input;

	// Create Buffer description
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(DirectionalLight);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;

	// Create the buffer
	HRESULT result;
	result = d3dDevice->CreateBuffer(&bufferDesc, NULL, &constantBuffer);
	
	// Check for error
	if (result != S_OK)
	{
		MessageBox(NULL, L"Failed to create directional Light constant buffer", L"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	return true;
}

void LightClass::Renderlights(XMFLOAT3 cameraPosition)
{
	// Create the light struct
	DirectionalLight directionalLight;
	directionalLight.ambient = XMFLOAT4(0.4f, 0.4f, 0.4f, 16.0f);
	directionalLight.diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	directionalLight.specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 16.0f);
	directionalLight.direction = XMFLOAT3(0, -1, -0.2);
	directionalLight.eyePos = XMFLOAT3(cameraPosition.x, cameraPosition.y, cameraPosition.z);
	directionalLight.useLight = useLight;

	// Update resources
	d3dContext->UpdateSubresource(constantBuffer, 0, NULL, &directionalLight, 0, 0);
	d3dContext->PSSetConstantBuffers(0, 1, &constantBuffer);
}

void LightClass::ToggleDirectionalLight(BOOL useLight)
{
}

void LightClass::UpdateLights()
{
	static bool isPressed = false;
	if (inputClass->IsKeyDown('L') && !isPressed)
		useLight = !useLight;

	isPressed = inputClass->IsKeyDown('L');

}