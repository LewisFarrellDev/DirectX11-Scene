#include "BillboardClass.h"
#include <algorithm>

BillboardClass::BillboardClass()
{
}

bool BillboardClass::InitializeBillboards(ID3D11Device *d3dDevice, ID3D11DeviceContext * d3dContext)
{
	InitializeModelClass(d3dDevice, d3dContext);
	return true;
}

void BillboardClass::UpdateModels(XMFLOAT3 cameraPosition)
{
	// Update the rotation angle based on working out an angle to rotate in proportion to time elapsed since last update
	DWORD CurrentTime = GetTickCount();
	// Calculate the difference in time
	DWORD diff = CurrentTime - rotationTime;

	// Set Cam position
	this->cameraPosition = cameraPosition;

	// Rotate Billboard Models
	for (int i = 0; i < modelList.size(); i++)
	{
		// Calculate Angle from model to camera
		double angle = atan2(modelList[i].position.x - cameraPosition.x, modelList[i].position.z - cameraPosition.z) * (180.0 / XM_PI);
		// Convert rotation into radians.
		rotation = (float)angle * 0.0174532925f;
		modelList[i].rotation.y = rotation;
	}

	rotationTime = CurrentTime;
}

void BillboardClass::RenderModels(XMMATRIX matWorld, XMMATRIX matView, XMMATRIX matProjection)
{
	// Store order of billboards
	std::vector<float> distances;
	for (int i = 0; i < modelList.size(); i++)
	{
		// Calculate distance for each billboard from the camera using Pythag

		// Calculate distance between camera and model (billboard)
		float xDistance = cameraPosition.x - modelList[i].position.x;
		float yDistance = cameraPosition.y - modelList[i].position.y;
		float zDistance = cameraPosition.z - modelList[i].position.z;

		// Get the cube root
		float distance = cbrt(pow(xDistance, 2) + pow(yDistance, 2) + pow(xDistance, 2));

		// Add distance to vector list
		distances.push_back(distance); // Add them to vector
	}

	// Sort by cloest to furthest away by distance
	std::sort(distances.begin(), distances.end());

	// Re arrange the models to be furtherst first
	std::vector<Model> tempModelList;

	// Add models to vector in descending order based on distance
	for (int i = distances.size()-1; i >= 0; i--)
	{
		for (int o = 0; 0 < modelList.size(); o++)
		{
			// Calculate distance between camera and model (billboard)
			float xDistance = cameraPosition.x - modelList[o].position.x;
			float yDistance = cameraPosition.y - modelList[o].position.y;
			float zDistance = cameraPosition.z - modelList[o].position.z;

			// Get the cube root
			float distance = cbrt(pow(xDistance, 2) + pow(yDistance, 2) + pow(xDistance, 2));

			// Compare
			if (distances[i] == distance)
			{
				// Add model to temp list
				tempModelList.push_back(modelList[o]);

				// Erase model from main list to prevent same model being included twice
				modelList.erase(modelList.begin() + o);
				break;
			}
		}
	}

	// Copy list over
	modelList = tempModelList;

	// Call parent function
	ModelClass::RenderModels(matWorld, matView, matProjection);
}

bool BillboardClass::InitializeObjects()
{
	Model model;

	// Flag
	if (LoadOBJ(L".\\Assets\\Models\\Model_Billboard.obj", &model))
	{
		model.SetTranslation(XMFLOAT3(0, 70, -160), XMFLOAT3(0, 0, 0), XMFLOAT3(10, 10, 10));
		model.SetTexture(L".\\Assets\\Textures\\Texture_Flag.png", d3dDevice, d3dContext);
		modelList.push_back(model);
		model = {};
	}

	return true;
}

bool BillboardClass::InitializeInstancedObjects()
{
	Model model;

	// Clown - Instanced from first billboard
	model.startingVertexIndex = modelList[0].startingVertexIndex;
	model.vertexCount = modelList[0].vertexCount;
	model.SetTranslation(XMFLOAT3(0, 10, -50), XMFLOAT3(0, 0, 0), XMFLOAT3(10, 10, 10));
	model.SetTexture(L".\\Assets\\Textures\\Texture_Clown.png", d3dDevice, d3dContext);
	modelList.push_back(model);
	model = {};

	// Target - Instanced from first billboard
	model.startingVertexIndex = modelList[0].startingVertexIndex;
	model.vertexCount = modelList[0].vertexCount;
	model.SetTranslation(XMFLOAT3(0, 36, 70), XMFLOAT3(0, 0, 0), XMFLOAT3(5, 5, 5));
	model.SetTexture(L".\\Assets\\Textures\\Texture_Target.png", d3dDevice, d3dContext);
	modelList.push_back(model);
	model = {};

	// Bell - Instanced from first billboard
	model.startingVertexIndex = modelList[0].startingVertexIndex;
	model.vertexCount = modelList[0].vertexCount;
	model.SetTranslation(XMFLOAT3(30, 20, 70), XMFLOAT3(0, 0, 0), XMFLOAT3(2.3f, 2, 2));
	model.SetTexture(L".\\Assets\\Textures\\Texture_Bell.png", d3dDevice, d3dContext);
	modelList.push_back(model);
	model = {};

	// Man - Instanced from first billboard
	model.startingVertexIndex = modelList[0].startingVertexIndex;
	model.vertexCount = modelList[0].vertexCount;
	model.SetTranslation(XMFLOAT3(0, 8, 65), XMFLOAT3(0, 0, 0), XMFLOAT3(4, 5.5f, 4));
	model.SetTexture(L".\\Assets\\Textures\\Texture_Man.png", d3dDevice, d3dContext);
	modelList.push_back(model);
	model = {};

	return true;
}
