#include "ModelClass.h"
#include <fstream>
#include <iostream>
#include <string>
#include <WICTextureLoader.h>

ModelClass::ModelClass()
{
}

bool ModelClass::LoadOBJ(LPCWSTR filePath, Model *model)
{
	using namespace std;

	// Open File
	ifstream file(filePath);

	// Temp Variables
	string tempString;
	size_t spacePosition;
	size_t slashPosition;
	int startPosition = 0;

	// Vectors
	vector<string> lines;
	vector<string> vertices;
	vector<string> t_normals;
	vector<string> faces;

	// Main Vectors
	vector< vector<string> > vertexPositions;
	vector< vector<string> > uvCoordinates;
	vector< vector<string> > textureNormals;
	vector< vector<string> > indexes;

	// Open the file and closse it after reading is done
	string line;
	while (std::getline(file, line))
	{
		lines.push_back(line);
	}
	file.close();

	// Read the OBJ File
	for (int i = 0; i < lines.size(); i++)
	{
		if (lines[i].length() < 1)
			continue;

		switch (lines[i].at(0))
		{
		case 'f':
			// Get Index of the space (We want to access the data after the F char)
			spacePosition = lines[i].find(' ') + 1;

			// Remove the letter from the front (Remove the F)
			tempString = lines[i].substr(spacePosition, lines[i].length());

			// Position of the first space (Store the position of the space so we can increment from here for more data)
			spacePosition = tempString.find(' ');

			// Position of first slash (Store the position of the slash so we know how to seperate the indivual numbers)
			slashPosition = tempString.find("/");

			// Get Each Value Seperatly
			for (int i = 0; i < 3; i++)
			{
				for (int o = 0; o < 3; o++)
				{
					faces.push_back(tempString.substr(startPosition, slashPosition - startPosition));
					startPosition = slashPosition + 1;
					slashPosition = tempString.find("/", startPosition);
					if (slashPosition > spacePosition)
						slashPosition = spacePosition;
				}

				indexes.push_back(faces);
				faces.clear();
				startPosition = slashPosition + 1;
				slashPosition = tempString.find("/", startPosition);
				spacePosition = tempString.find(' ', startPosition);
			}
			startPosition = 0;

			break;

		case 'v':
			switch (lines[i].at(1))
			{
			case 't':
				// Get Index of the space
				spacePosition = lines[i].find(' ') + 1;

				// Remove the letter from the front
				tempString = lines[i].substr(spacePosition, lines[i].length());

				// Position of the first space
				spacePosition = tempString.find(' ');

				// Get Each Value Seperatly
				for (int i = 0; i < 2; i++)
				{
					spacePosition = tempString.find(' ', startPosition);
					t_normals.push_back(tempString.substr(startPosition, spacePosition - startPosition));
					startPosition = spacePosition + 1;
				}
				uvCoordinates.push_back(t_normals);
				t_normals.clear();
				startPosition = 0;
				break;

			case 'n':
				// Get Index of the space
				spacePosition = lines[i].find(' ') + 1;

				// Remove the letter from the front
				tempString = lines[i].substr(spacePosition, lines[i].length());

				// Position of the first space
				spacePosition = tempString.find(' ');

				// Get Each Value Seperatly
				for (int i = 0; i < 3; i++)
				{
					spacePosition = tempString.find(' ', startPosition);
					t_normals.push_back(tempString.substr(startPosition, spacePosition - startPosition));
					startPosition = spacePosition + 1;
				}
				textureNormals.push_back(t_normals);
				t_normals.clear();
				startPosition = 0;
				break;

			default:
				// Get Index of the space
				spacePosition = lines[i].find(' ') + 1;

				// Remove the letter from the front
				tempString = lines[i].substr(spacePosition, lines[i].length());

				// Position of the first space
				spacePosition = tempString.find(' ');

				// Add to the vertex Count (May be useful later)
				//model->vertexCount++;

				// Get Each Value Seperatly
				for (int i = 0; i < 3; i++)
				{
					spacePosition = tempString.find(' ', startPosition);
					vertices.push_back(tempString.substr(startPosition, spacePosition - startPosition));
					startPosition = spacePosition + 1;
				}
				vertexPositions.push_back(vertices);
				vertices.clear();
				startPosition = 0;
				break;
			}

		}
	}

	// If file was read, input data to struct
	if (lines.size() != 0)
	{
		vector<Vertex> shape;

		// Create the shape
		for (int i = indexes.size() - 1; i >= 0; i--)
		{
			Vertex temp;

			if (indexes[i][0].size() > 0)
			{
				// Vertex Position
				// Read the first value from each index Vector
				// Use that number as the index for the Vertex Vector
				int positionIndex = stoi(indexes[i][0]) - 1;
				float pX = stof(vertexPositions[positionIndex][0]);
				float pY = stof(vertexPositions[positionIndex][1]);
				float pZ = stof(vertexPositions[positionIndex][2]) * -1; // Invert to accomdate for left handed coordinate system
				temp.pos = XMFLOAT3(pX, pY, pZ);
			}

			if (indexes[i][1].size() > 0)
			{
				// UV Coordinates
				// Read the Second value from each index Vector
				// Use that number as the index for the UV Vector
				int uvIndex = stoi(indexes[i][1]) - 1;
				float uX = stof(uvCoordinates[uvIndex][0]);
				float uY = 1 - stof(uvCoordinates[uvIndex][1]);
				temp.texcoord = XMFLOAT2(uX, uY);
			}

			if (indexes[i][2].size() > 0)
			{
				// Normal Direction
				// Read the third value from each index Vector
				// Use that number as the index for the Normal Vector
				int normalIndex = stoi(indexes[i][2]) - 1;
				float nX = stof(textureNormals[normalIndex][0]);
				float nY = stof(textureNormals[normalIndex][1]);
				float nZ = stof(textureNormals[normalIndex][2]) * -1; // Invert to accomdate for left handed coordinate system
				temp.normal = XMFLOAT3(nX, nY, nZ);
			}

			shape.push_back(temp);
		}

		// Create the Object Struct to store all the details for this model
		model->vertices = shape;

		return true;
	}
	return false;
}

bool ModelClass::InitializeObjects()
{
	// temp Storage for loaded model
	Model model;

	// Load Models
	// SkyBox - 0
	if (LoadOBJ(L".\\Assets\\Models\\Model_SkyBox.obj", &model))
	{
		model.SetTranslation(XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(1000, 1000, 1000));
		model.SetTexture(L".\\Assets\\Textures\\Texture_CubeMap_1.jpg", d3dDevice, d3dContext);
		modelList.push_back(model);
		model = {};
	}

	// *********** Models *****************
	// Circus - 1;
	if (LoadOBJ(L".\\Assets\\Models\\Model_Circus.obj", &model))
	{
		model.SetTranslation(XMFLOAT3(0, -7, -160), XMFLOAT3(0, 0, 0), XMFLOAT3(12, 10, 12));
		model.SetTexture(L".\\Assets\\Textures\\Texture_Atlas.png", d3dDevice, d3dContext);
		modelList.push_back(model);
		model = {};
	}

	// ShootingRange - 2
	if (LoadOBJ(L".\\Assets\\Models\\Model_ShootingRange.obj", &model))
	{
		model.SetTranslation(XMFLOAT3(0, 7, 70), XMFLOAT3(0, 0, 0), XMFLOAT3(3, 3, 3));
		model.SetTexture(L".\\Assets\\Textures\\Texture_Atlas.png", d3dDevice, d3dContext);
		modelList.push_back(model);
		model = {};
	}

	// Swinging Ride - 3	
	if (LoadOBJ(L".\\Assets\\Models\\Model_SpinningSwing.obj", &model))
	{
		model.SetTranslation(XMFLOAT3(160, -2, -20), XMFLOAT3(0, 0, 0), XMFLOAT3(4, 4, 4));
		model.SetTexture(L".\\Assets\\Textures\\Texture_Atlas.png", d3dDevice, d3dContext);
		modelList.push_back(model);
		model = {};
	}

	// Hammer Game - 4
	if (LoadOBJ(L".\\Assets\\Models\\Model_HammerGame.obj", &model))
	{
		model.SetTranslation(XMFLOAT3(30, 7, 70), XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1));
		model.SetTexture(L".\\Assets\\Textures\\Texture_Atlas.png", d3dDevice, d3dContext);
		modelList.push_back(model);
		model = {};
	}

	// Frog Hopper - 5
	if (LoadOBJ(L".\\Assets\\Models\\Model_FrogHopper.obj", &model))
	{
		model.SetTranslation(XMFLOAT3(-70, 2, 70), XMFLOAT3(0, 0, 0), XMFLOAT3(3.5f, 3.5f, 3.5f));
		model.SetTexture(L".\\Assets\\Textures\\Texture_Atlas.png", d3dDevice, d3dContext);
		modelList.push_back(model);
		model = {};
	}

	// Ferris Wheel Frame - 6
	if (LoadOBJ(L".\\Assets\\Models\\Model_FerrisWheel_Frame.obj", &model))
	{
		model.SetTranslation(XMFLOAT3(-150, 103, -30), XMFLOAT3(0, 0, 0), XMFLOAT3(10, 10, 10));
		model.SetTexture(L".\\Assets\\Textures\\Texture_Atlas.png", d3dDevice, d3dContext);
		modelList.push_back(model);
		model = {};
	}

	// Ferris Wheel Wheel - 7
	if (LoadOBJ(L".\\Assets\\Models\\Model_FerrisWheel_Stand.obj", &model))
	{
		model.SetTranslation(XMFLOAT3(-150, -2, -30), XMFLOAT3(0, 90, 0), XMFLOAT3(10, 10, 10));
		model.SetTexture(L".\\Assets\\Textures\\Texture_Atlas.png", d3dDevice, d3dContext);
		modelList.push_back(model);
		model = {};
	}

	// Train Tracks - 8
	if (LoadOBJ(L".\\Assets\\Models\\Model_Train_Tracks.obj", &model))
	{
		model.SetTranslation(XMFLOAT3(0, 5, 180), XMFLOAT3(0, 0, 0), XMFLOAT3(5, 5, 5));
		model.SetTexture(L".\\Assets\\Textures\\Texture_Atlas.png", d3dDevice, d3dContext);
		modelList.push_back(model);
		model = {};
	}

	// Train Tracks - 9
	if (LoadOBJ(L".\\Assets\\Models\\Model_Train.obj", &model))
	{
		model.SetTranslation(XMFLOAT3(0, 5, 180), XMFLOAT3(0, 0, 0), XMFLOAT3(5, 5, 5));
		model.SetTexture(L".\\Assets\\Textures\\Texture_Atlas.png", d3dDevice, d3dContext);
		modelList.push_back(model);
		model = {};
	}

	// Spin Tower - 10
	if (LoadOBJ(L".\\Assets\\Models\\Model_SpinTower.obj", &model))
	{
		model.SetTranslation(XMFLOAT3(150, 0, -150), XMFLOAT3(0, 0, 0), XMFLOAT3(10, 10, 10));
		model.SetTexture(L".\\Assets\\Textures\\Texture_Atlas.png", d3dDevice, d3dContext);
		modelList.push_back(model);
		model = {};
	}

	// Spin Tower cylinder - 11
	if (LoadOBJ(L".\\Assets\\Models\\Model_SpinTower_Cylinder.obj", &model))
	{
		model.SetTranslation(XMFLOAT3(150, 0, -150), XMFLOAT3(0, 0, 0), XMFLOAT3(10, 10, 10));
		model.SetTexture(L".\\Assets\\Textures\\Texture_Atlas.png", d3dDevice, d3dContext);
		modelList.push_back(model);
		model = {};
	}

	// Pirate Ship - 12
	if (LoadOBJ(L".\\Assets\\Models\\Model_PirateShip.obj", &model))
	{
		model.SetTranslation(XMFLOAT3(150, 70, 100), XMFLOAT3(0, 0, 0), XMFLOAT3(1.5f, 1.5f, 1.5f));
		model.SetTexture(L".\\Assets\\Textures\\Texture_Atlas.png", d3dDevice, d3dContext);
		modelList.push_back(model);
		model = {};
	}

	// Pirate Ship Frame - 13
	if (LoadOBJ(L".\\Assets\\Models\\Model_PirateShip_Frame.obj", &model))
	{
		model.SetTranslation(XMFLOAT3(150, 11, 100), XMFLOAT3(0, 0, 0), XMFLOAT3(1.5f, 1.5f, 1.5f));
		model.SetTexture(L".\\Assets\\Textures\\Texture_Atlas.png", d3dDevice, d3dContext);
		modelList.push_back(model);
		model = {};
	}

	// Park Bench - 14
	if (LoadOBJ(L".\\Assets\\Models\\Model_ParkBench.obj", &model))
	{
		model.SetTranslation(XMFLOAT3(-50, 2, 150), XMFLOAT3(0, 0, 0), XMFLOAT3(3, 3, 3));
		model.SetTexture(L".\\Assets\\Textures\\Texture_Atlas.png", d3dDevice, d3dContext);
		modelList.push_back(model);
		model = {};
	}

	return true;
}

bool ModelClass::InitializeInstancedObjects()
{
	// Create Instanced Models Here
	Model model;

	// Create Multiple Park Benches based of the orignal
	if (modelList.size() > 14)
	{
		int posX = -20;
		for (int i = 0; i < 5; i++)
		{
			model.vertexCount = modelList[14].vertexCount;
			model.startingVertexIndex = modelList[14].startingVertexIndex;
			model.SetTranslation(XMFLOAT3(posX, 2, 150), XMFLOAT3(0, 0, 0), XMFLOAT3(3, 3, 3));
			model.texture = modelList[14].texture;
			modelList.push_back(model);
			posX += 30;
			model = {};
		}
	}

	if (modelList.size() > 6)
	{
		int posX = 5;
		for (int i = 0; i < 4; i++)
		{
			// Ferris Wheel Wheel
			model.vertexCount = modelList[6].vertexCount;
			model.vertexCount = modelList[6].vertexCount;
			model.startingVertexIndex = modelList[6].startingVertexIndex;
			model.position = XMFLOAT3(-20, 8, 79);
			model.position.y += 3.1;
			model.position.x += posX;
			model.scale = XMFLOAT3(0.3, 0.3, 0.3);
			model.SetRotation(XMFLOAT3(0, 90, 0));
			model.texture = modelList[6].texture;
			modelList.push_back(model);
			model = {};

			// Ferris Wheel Frame
			model.vertexCount = modelList[7].vertexCount;
			model.vertexCount = modelList[7].vertexCount;
			model.startingVertexIndex = modelList[7].startingVertexIndex;
			model.position = XMFLOAT3(-20, 8, 79);
			model.position.x += posX;
			model.scale = XMFLOAT3(0.3, 0.3, 0.3);
			model.SetRotation(XMFLOAT3(0, 0, 0));
			model.texture = modelList[7].texture;
			modelList.push_back(model);
			model = {};

			posX += 10;
		}
	}

	return true;
}

bool ModelClass::InitializeModelClass(ID3D11Device *d3dDevice, ID3D11DeviceContext * d3dContext)
{
	// Store the device & Context
	this->d3dDevice = d3dDevice;
	this->d3dContext = d3dContext;

	if (!InitializeObjects())
		return false;

	// All the vertices for init data
	UINT bufferSize = 1; // Used to calculate buffer size
	std::vector <Vertex> initDataVertexes;

	// Get the buffer size
	for (int i = 0; i < modelList.size(); i++)
	{
		modelList[i].startingVertexIndex = bufferSize - 1;
		modelList[i].vertexCount = modelList[i].vertices.size();
		bufferSize += modelList[i].vertices.size();

		// Load up all the vertices for the init data
		initDataVertexes.insert(initDataVertexes.end(), modelList[i].vertices.begin(), modelList[i].vertices.end());

		// Clear the vertices, no longer needed as it will be stored in the buffer (Disabled for water)
		//modelList[i].vertices.clear();
		//modelList[i].vertices.shrink_to_fit();
	}

	// If there are no vertices, make an empty to initialise init data
	if (initDataVertexes.size() == 0)
	{
		Model tempShape;
		Vertex tempVertex =
		{ {-1.00000000f, 1.00000000f, 1.00000000f},
		{ 0.251249999f, 0.334167004f },
		{ 0.000000000f, 0.00000000f, 0.0000000f } };
		tempShape.vertices.push_back(tempVertex);
		initDataVertexes = tempShape.vertices;
	}

	// Create Buffer Description
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.ByteWidth = sizeof(Vertex) * bufferSize;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	// Create Initial Data
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = initDataVertexes.data();

	// Create the vertex Buffer
	HRESULT result = d3dDevice->CreateBuffer(&bufferDesc, &InitData, &vertexBuffer);
	if (result != S_OK)
	{
		MessageBox(NULL, L"Failed to create vertex buffer (ModelClass.CPP)", L"Error", MB_OK | MB_ICONERROR);
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
		MessageBox(NULL, L"Failed to create constant buffer (ModelClass.CPP)", L"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	uniqueModelCount = modelList.size();

	if (!InitializeInstancedObjects())
		return false;

	return true;
}

void ModelClass::RenderModels(XMMATRIX matWorld, XMMATRIX matView, XMMATRIX matProjection)
{
	for (int i = 0; i < modelList.size(); i++)
	{
		// Translation of the Model
		matWorld = XMMatrixIdentity() *
			XMMatrixRotationX(modelList[i].rotation.x) *
			XMMatrixRotationY(modelList[i].rotation.y) *
			XMMatrixRotationZ(modelList[i].rotation.z) *
			XMMatrixScaling(modelList[i].scale.x, modelList[i].scale.y, modelList[i].scale.z) *
			XMMatrixTranslation(modelList[i].position.x, modelList[i].position.y, modelList[i].position.z);

		ConstantBuffer tempConstantBuffer;
		tempConstantBuffer.mWorld = XMMatrixTranspose(matWorld);
		tempConstantBuffer.mView = XMMatrixTranspose(matView);
		tempConstantBuffer.mProjection = XMMatrixTranspose(matProjection);
		d3dContext->UpdateSubresource(constantBuffer, 0, NULL, &tempConstantBuffer, 0, 0);

		// Set Texture
		d3dContext->PSSetShaderResources(1, 1, &modelList[i].texture);

		// Set the constant Buffer
		d3dContext->VSSetConstantBuffers(0, 1, &constantBuffer);

		// Have a valid vertex buffer at this point.  Now need to pass this to the input assembler of the pipeline
		// (1st stage) to begin rendering
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		d3dContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

		// Also tell input assembler how the vertices are to be treated.
		d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Draw the model
		d3dContext->Draw(modelList[i].vertexCount, modelList[i].startingVertexIndex);
	}
}

void ModelClass::ShutdownModels()
{
	if (vertexBuffer)
		vertexBuffer->Release();

	if (constantBuffer)
		constantBuffer->Release();
}

void ModelClass::UpdateModels(XMFLOAT3 cameraPosition)
{
	// Update the rotation angle based on working out an angle to rotate in proportion to time elapsed since last update
	DWORD CurrentTime = GetTickCount();
	// Calculate the difference in time
	DWORD diff = CurrentTime - rotationTime;

	// Move Skybox with camera
	if (modelList.size() > 0)
		modelList[0].position = cameraPosition;

	// Rotate Swing
	if (modelList.size() > 4)
	{
		modelList[3].rotation.y += (360 * XM_PI / 180) * (diff / 4000.0f);
		// Reset when done 360 degrees
		if (modelList[3].rotation.y > 360 * XM_PI / 180) {
			modelList[3].rotation.y = 0.0f;
		}
	}
	
	// Rotate Ferris Wheel frame
	if (modelList.size() > 6)
	{
		modelList[6].rotation.x += (360 * XM_PI / 180) * (diff / 8000.0f);
		// Reset when done 360 degrees
		if (modelList[6].rotation.x > 360 * XM_PI / 180) {
			modelList[6].rotation.x = 0.0f;
		}
	}

	// Move train
	if (modelList.size() > 9)
	{
		if (modelList[9].position.x > 100)
		{
			trainMaxDistance = true;
			modelList[9].position.x = 99.5f;
			modelList[9].rotation.y += 180 * (XM_PI / 180);
		}
		else if (modelList[9].position.x < -100)
		{
			trainMaxDistance = false;
			modelList[9].position.x = -99.5f;
			modelList[9].rotation.y -= 180 * (XM_PI / 180);
		}

		if (!trainMaxDistance)
			modelList[9].position.x += 25 * (diff / 1000.0f);
		else
		{
			modelList[9].position.x -= 25 * (diff / 1000.0f);
		}
	}

	// Rotate tower cyclinder
	if (modelList.size() > 11)
	{
		modelList[11].rotation.y += (360 * XM_PI / 180) * (diff / 12000.0f);
		// Rotate clyinder
		if (modelList[11].rotation.y > 360 * XM_PI / 180) {
			modelList[11].rotation.y = 0.0f;
		}

		// Move tower Cyclinder
		if (modelList[11].position.y > 190)
		{
			spinTowerMaxHeight = true;
			modelList[11].position.y = 189.5f;
		}
		else if (modelList[11].position.y < 0)
		{
			spinTowerMaxHeight = false;
			modelList[11].position.y = 0.5f;
		}

		if (!spinTowerMaxHeight)
			modelList[11].position.y += 10 * (diff / 1000.0f);
		else
		{
			modelList[11].position.y += -10 * (diff / 1000.0f);
		}
	}

	// Swing pirate ship
	if (modelList.size() > 12)
	{
		// Sin result in radians
		double frequency = 1; // Number of waves per second
		double sinResult = sin(CurrentTime * (frequency / 1000.0)) * 85; // First bracket = Frequency. Second Bracket = Amplitude
		modelList[12].SetRotation(XMFLOAT3(modelList[12].rotation.x, modelList[12].rotation.y, sinResult));
	}

	rotationTime = CurrentTime;
}

void Model::SetTranslation(XMFLOAT3 position, XMFLOAT3 rotation, XMFLOAT3 scale)
{
	this->position = position;
	this->scale = scale;
	this->rotation = XMFLOAT3(rotation.x * XM_PI / 180, rotation.y * XM_PI / 180, rotation.z * XM_PI / 180);
}

void Model::SetRotation(XMFLOAT3 rotation)
{
	this->rotation = XMFLOAT3(rotation.x * XM_PI / 180, rotation.y * XM_PI / 180, rotation.z * XM_PI / 180);
}

bool Model::SetTexture(LPCWSTR texturePath, ID3D11Device * d3dDevice, ID3D11DeviceContext * d3dContext)
{
	HRESULT result = CreateWICTextureFromFile(d3dDevice, d3dContext, texturePath, NULL, &this->texture);

	// Check texture was loaded successfully
	if (result != S_OK)
	{
		MessageBox(NULL, L"Error Loading Texture File (ModelClass.CPP)", texturePath, MB_OK | MB_ICONERROR);
		return false;
	}

	return true;
}
