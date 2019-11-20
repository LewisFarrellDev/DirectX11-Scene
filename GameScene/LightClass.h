#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

struct DirectionalLight
{
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT4 specular;
	XMFLOAT3 direction;
	XMFLOAT3 eyePos;
	float pad1;
	BOOL useLight;
};

class LightClass
{
public:
	LightClass();
	bool InitializeLightClass(ID3D11Device *d3dDevice, ID3D11DeviceContext * d3dContext, class InputClass *input);
	void UpdateLights();
	void Renderlights(XMFLOAT3 cameraPosition);
	void ToggleDirectionalLight(BOOL lightToggle);

private:
	ID3D11Buffer *constantBuffer = NULL;

	ID3D11Device *d3dDevice = NULL;
	ID3D11DeviceContext *d3dContext = NULL;

	bool useLight;
	class InputClass *inputClass;
};

