#include "DynamicModelClass.h"

DynamicModelClass::DynamicModelClass()
{
}


bool DynamicModelClass::InitializeObjects()
{
	Model model;

	// Water
	if (LoadOBJ(L".\\Assets\\Models\\Model_WaterPlane.obj", &model))
	{
		model.SetTranslation(XMFLOAT3(0, -11, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(1000, 15, 1000));
		model.SetTexture(L".\\Assets\\Textures\\Texture_Water.png", d3dDevice, d3dContext);
		modelList.push_back(model);
		model = {};
	}

	// Magic Carpet
	if (LoadOBJ(L".\\Assets\\Models\\Model_MagicCarpet.obj", &model))
	{
		model.SetTranslation(XMFLOAT3(0, 11, -50), XMFLOAT3(0, 0, 0), XMFLOAT3(10, 4, 10));
		model.SetTexture(L".\\Assets\\Textures\\Texture_Carpet.jpg", d3dDevice, d3dContext);
		modelList.push_back(model);
		model = {};
	}

	return true;
}

bool DynamicModelClass::InitializeInstancedObjects()
{
	return true;
}

bool DynamicModelClass::InitializeDynamicModels(ID3D11Device * d3dDevice, ID3D11DeviceContext * d3dContext)
{
	InitializeModelClass(d3dDevice, d3dContext);
	return true;
}

void DynamicModelClass::UpdateModels(XMFLOAT3 cameraPosition)
{
	// Update the rotation angle based on working out an angle to rotate in proportion to time elapsed since last update
	DWORD CurrentTime = GetTickCount();
	// Calculate the difference in time
	DWORD diff = CurrentTime - rotationTime;

	// Move Water
	if (modelList.size() > 0)
	{
		for (int o = 0; o < modelList[0].vertices.size(); o++)
		{
			double sinResult = sin((CurrentTime * 0.001) + (modelList[0].vertices[o].pos.x * 10) + (modelList[0].vertices[o].pos.z * 2)) * 0.1;
			modelList[0].vertices[o].pos.y = sinResult;
		}
	}

	// Move Carpet
	if (modelList.size() > 1)
	{
		for (int o = 0; o < modelList[1].vertices.size(); o++)
		{
			double sinResult = cos((CurrentTime * 0.01) + (modelList[1].vertices[o].pos.x) * 2) * 0.1;
			modelList[1].vertices[o].pos.y = sinResult;
		}
	}

	// RE POPULATE VERTEX BUFFER
	// All the vertices for init data
	if (uniqueModelCount > 0)
	{
		UINT bufferSize = 1; // Used to calculate buffer size
		std::vector <Vertex> initDataVertexes;

		// Get the buffer size
		for (int i = 0; i < uniqueModelCount; i++)
		{
			modelList[i].startingVertexIndex = bufferSize - 1;
			modelList[i].vertexCount = modelList[i].vertices.size();
			bufferSize += modelList[i].vertices.size();

			// Load up all the vertices for the init data
			initDataVertexes.insert(initDataVertexes.end(), modelList[i].vertices.begin(), modelList[i].vertices.end());
		}

		D3D11_MAPPED_SUBRESOURCE resource;
		d3dContext->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		memcpy(resource.pData, initDataVertexes.data(), sizeof(Vertex) * bufferSize);
		d3dContext->Unmap(vertexBuffer, 0);
	}

	rotationTime = CurrentTime;
}
