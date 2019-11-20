#include "TerrainClass.h"
#include <WICTextureLoader.h>

TerrainClass::TerrainClass()
{

}

bool TerrainClass::InitializeTerrain(ID3D11Device * d3dDevice, ID3D11DeviceContext * d3dContext)
{
	this->d3dDevice = d3dDevice;
	this->d3dContext = d3dContext;

	CreateTerrainFromHeightMap(".\\Assets\\Textures\\Terrain_Heightmap.bmp", heightMap);
	heightMap.scale = XMFLOAT3(350, 1.5f, 350);
	heightMap.position = XMFLOAT3(0, -14, 0);
	heightMap.SetTexture(L".\\Assets\\Textures\\Texture_Plane.jpg", d3dDevice, d3dContext);

	// Create Buffer Description
	// Vertex Buffer
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.ByteWidth = sizeof(Vertex) * heightMap.vertices.size();
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;

	// Create Initial Data
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = heightMap.vertices.data();

	// Create the vertex Buffer
	HRESULT result = d3dDevice->CreateBuffer(&bufferDesc, &InitData, &vertexBuffer);
	if (result != S_OK)
	{
		MessageBox(NULL, L"Failed to create vertex buffer (TerrainClass.CPP)", L"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	//Define the index buffer
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = sizeof(int) * numIndices;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;

	// Needs access to the triangle structure defined above
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = indices;

	result = d3dDevice->CreateBuffer(&bufferDesc, &InitData, &indexBuffer);

	if (result != S_OK)
	{
		MessageBox(NULL, L"Failed to create Index buffer (TerrainClass.CPP)", L"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	// Create the constant buffer
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(ConstantBuffer);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	result = d3dDevice->CreateBuffer(&bufferDesc, NULL, &constantBuffer);
	if (result != S_OK)
	{
		MessageBox(NULL, L"Failed to create constant buffer (TerrainClass.CPP)", L"Error", MB_OK | MB_ICONERROR);
		return false;
	}


	return true;
}

void TerrainClass::RenderTerrain(XMMATRIX matWorld, XMMATRIX matView, XMMATRIX matProjection)
{
	// Translation of the Model
	matWorld = XMMatrixIdentity() *
		XMMatrixScaling(heightMap.scale.x, heightMap.scale.y, heightMap.scale.z) *
		XMMatrixTranslation(heightMap.position.x, heightMap.position.y, heightMap.position.z);

	ConstantBuffer tempConstantBuffer;
	tempConstantBuffer.mWorld = XMMatrixTranspose(matWorld);
	tempConstantBuffer.mView = XMMatrixTranspose(matView);
	tempConstantBuffer.mProjection = XMMatrixTranspose(matProjection);
	d3dContext->UpdateSubresource(constantBuffer, 0, NULL, &tempConstantBuffer, 0, 0);

	// Set Texture
	d3dContext->PSSetShaderResources(1, 1, &heightMap.texture);

	// Set the constant Buffer
	d3dContext->VSSetConstantBuffers(0, 1, &constantBuffer);

	// Have a valid vertex buffer at this point.  Now need to pass this to the input assembler of the pipeline
	// (1st stage) to begin rendering
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	d3dContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	d3dContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Also tell input assembler how the vertices are to be treated.
	d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Draw the model
	d3dContext->DrawIndexed(heightMap.vertices.size() * 6, 0, 0);
}

bool TerrainClass::CreateTerrainFromHeightMap(char * filename, HeightMapInfo & heightMap)
{
	// Open the heightmap file
	FILE *file = fopen(filename, "rb");
	if (file == NULL)
		return false;

	// Read the information from the file
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER bitmapInfoHeader;

	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, file);
	fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, file);

	// Store some of the information
	heightMap.terrainWidth = bitmapInfoHeader.biWidth;
	heightMap.terrainHeight = bitmapInfoHeader.biHeight;

	// Local variable for image size
	int imageSize = heightMap.terrainWidth * heightMap.terrainHeight;

	// Move file pointer to start of pixel colour data
	fseek(file, bitmapFileHeader.bfOffBits, SEEK_SET);

	// Work out number of bytes per pixel for array
	int numBytesPerPixel = bitmapInfoHeader.biBitCount / 8;

	// Calculate maximum colour value up 2^24 colour
	float MaxColourVal = (float)pow(2.0f, min(24.0f, (float)bitmapInfoHeader.biBitCount));

	// Create array for pixel data
	unsigned char *pixel_data = new unsigned char[imageSize * numBytesPerPixel];

	// Read pixel data from file into memory
	fread(pixel_data, sizeof(unsigned char), imageSize * numBytesPerPixel, file);

	// Release the file
	fclose(file);

	// Create The Grid Here using height and width as parameters
	CreateTerrainGrid(heightMap);

	// Create Vertices to be used for terrain
	int idx = 0;
	float maxHeight = 20;
	for (int row = 0; row < heightMap.terrainHeight; row++)
	{
		for (int col = 0; col < heightMap.terrainWidth; col++)
		{
			int pixel_val = pixel_data[idx * numBytesPerPixel]
				+ (pixel_data[(idx * numBytesPerPixel) + 1] << 8)
				+ (pixel_data[(idx * numBytesPerPixel) + 2] << 16);

			heightMap.vertices[idx].pos.y = maxHeight * (pixel_val / MaxColourVal);

			idx++;
		}
	}

	return true;
}

bool TerrainClass::CreateTerrainGrid(HeightMapInfo &heightMap)
{
	//Calculate the total number of vertices and create array to store vertices
	vertexCount = heightMap.terrainHeight * heightMap.terrainWidth;
	//*mesh = new Vertex[*TotalNumVert];

	//Calculate step between vertices
	float zStep = 2.0f / heightMap.terrainHeight;
	float xStep = 2.0f / heightMap.terrainWidth;

	// Starting point of terrain -1.0f, 0.0f, -1.0f
	float xPos = -1.0f;
	float zPos = -1.0f;

	//Calculate the uv step
	float uvZStep = 1.0f / heightMap.terrainHeight;
	float uvXStep = 1.0f / heightMap.terrainWidth;

	// Starting point of uv 0.0f, 0.0f
	float u = 0.0f;
	float v = 0.0f;

	int idx = 0;

	//Create vertices for terrain based on NumVertices
	for (int row = 0; row < heightMap.terrainHeight; row++)
	{
		for (int col = 0; col < heightMap.terrainWidth; col++)
		{
			Vertex tempVertex;
			tempVertex.pos = XMFLOAT3(xPos, 0, zPos);
			tempVertex.normals = XMFLOAT3(0, 1, 0);
			tempVertex.texcoord = XMFLOAT2(u, v);
			heightMap.vertices.push_back(tempVertex);

			xPos += xStep;
			u += uvXStep;
			idx++;
		}

		u = 0.0f;
		v += uvZStep;
		xPos = -1.0f;
		zPos += zStep;
	}



	// Create index array.  Note that the number of indices
	// are reflective of the number of triangles not lines
	// in grid, hence 1 less than the number of lines
	numIndices = (heightMap.terrainHeight - 1) * (heightMap.terrainWidth - 1) * 6;
	indices = new int[numIndices];

	int *p = indices;

	// Define the first two triangles

	p[0] = 0;
	p[1] = heightMap.terrainHeight;
	p[2] = heightMap.terrainHeight + 1;
	p[3] = heightMap.terrainHeight + 1;
	p[4] = 1;
	p[5] = 0;


	//Now create the other triangles 
	int NextRow = (heightMap.terrainHeight - 1) * 6;
	for (int idx = 6; idx < numIndices; idx++)
	{
		if (idx % NextRow == 0)
		{
			p[idx] = (idx / NextRow) * heightMap.terrainHeight;
			p[idx + 1] = p[idx] + heightMap.terrainHeight;
			p[idx + 2] = p[idx + 1] + 1;
			p[idx + 3] = p[idx + 2];
			p[idx + 4] = p[idx] + 1;
			p[idx + 5] = p[idx];

			idx += 5;
		}
		else
		{
			p[idx] = p[idx - 6] + 1;
		}
	}

	return true;

}

bool TerrainClass::HeightMapInfo::SetTexture(LPCWSTR texturePath, ID3D11Device * d3dDevice, ID3D11DeviceContext * d3dContext)
{
	HRESULT result = CreateWICTextureFromFile(d3dDevice, d3dContext, texturePath, NULL, &this->texture);

	// Check texture was loaded successfully
	if (result != S_OK)
	{
		MessageBox(NULL, L"Error Loading Texture File (Terrain Class.CPP)", texturePath, MB_OK | MB_ICONERROR);
		return false;
	}

	return true;
}
