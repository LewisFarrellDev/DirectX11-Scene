#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>

using namespace DirectX;

class TerrainClass
{
	struct ConstantBuffer
	{
		XMMATRIX mWorld;
		XMMATRIX mView;
		XMMATRIX mProjection;
	};

	struct Vertex
	{
		XMFLOAT3 pos;
		XMFLOAT2 texcoord;
		XMFLOAT3 normals;
	};

	struct HeightMapInfo
	{
		int terrainWidth;
		int terrainHeight;
		ID3D11ShaderResourceView *texture;
		std::vector <Vertex> vertices;
		XMFLOAT3 scale;
		XMFLOAT3 position;

		bool SetTexture(LPCWSTR texturePath, ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dContext);
	};
		
public:
	TerrainClass();
	bool InitializeTerrain(ID3D11Device * d3dDevice, ID3D11DeviceContext * d3dContext);
	void RenderTerrain(XMMATRIX matWorld, XMMATRIX matView, XMMATRIX matProjection);

private:
	bool CreateTerrainFromHeightMap(char *filename, HeightMapInfo &heightMap);
	bool CreateTerrainGrid(HeightMapInfo &heightMap);

	ID3D11Device *d3dDevice = NULL;
	ID3D11DeviceContext *d3dContext = NULL;

	ID3D11Buffer *vertexBuffer = NULL;
	ID3D11Buffer *constantBuffer = NULL;
	ID3D11Buffer *indexBuffer = NULL;

	HeightMapInfo heightMap;

	int vertexCount;
	int faceCount;
	int numIndices;
	int *indices;
};

