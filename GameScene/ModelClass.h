#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>

using namespace DirectX;

struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT2 texcoord;
	XMFLOAT3 normal;
};

struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
};

struct Model
{
	std::vector <Vertex> vertices;
	XMFLOAT3 position = XMFLOAT3(0, 0, 0);
	XMFLOAT3 scale = XMFLOAT3(1,1,1);
	XMFLOAT3 rotation = XMFLOAT3(0, 0, 0);
	ID3D11ShaderResourceView *texture;
	int startingVertexIndex = 0;
	int vertexCount = 0;

	// Position, Rotation, Scale
	void SetTranslation(XMFLOAT3 position, XMFLOAT3 rotation, XMFLOAT3 scale);
	// Set rotation in degrees
	void SetRotation(XMFLOAT3 rotation);
	bool SetTexture(LPCWSTR texturePath, ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dContext);
};

class ModelClass
{
public:
	ModelClass();
	bool InitializeModelClass(ID3D11Device *d3dDevice, ID3D11DeviceContext * d3dContext);
	virtual void RenderModels(XMMATRIX matWorld, XMMATRIX matView, XMMATRIX matProjection);
	virtual void UpdateModels(XMFLOAT3 cameraPosition);
	void ShutdownModels();	

protected:
	ID3D11Buffer *vertexBuffer = NULL;
	ID3D11Buffer *constantBuffer = NULL;
	ID3D11Device *d3dDevice = NULL;
	ID3D11DeviceContext *d3dContext = NULL;

	std::vector <Model> modelList;
	int uniqueModelCount;

	DWORD rotationTime = 0;
	float rotation = 0.0f;

	bool LoadOBJ(LPCWSTR filePath, Model *model);
	virtual bool InitializeObjects();
	virtual bool InitializeInstancedObjects();

private:
	bool trainMaxDistance = false;
	bool spinTowerMaxHeight = false;
};

