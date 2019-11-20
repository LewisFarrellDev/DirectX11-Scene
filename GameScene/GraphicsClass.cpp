#include "GraphicsClass.h"
#include "InputClass.h"

GraphicsClass::GraphicsClass()
{
}

bool GraphicsClass::InitializeGraphics(HWND windowHandle, InputClass *inputClass)
{
	// Store reference to window Handle
	this->windowHandle = windowHandle;
	this->inputClass = inputClass;

	// Create a new directX Class, if it fails return false
	directXClass = new DirectXClass();
	if (directXClass)
		if (!directXClass->InitializeDirectX(windowHandle))
			return false;

	// Create Camera Class
	cameraClass = new CameraClass();
	if (cameraClass)
		if (!cameraClass->InitialiseCamera(windowHandle, inputClass))
			return false;

	// Create a new model class, if it fails return false
	modelClass = new ModelClass();
	if (modelClass)
		if (!modelClass->InitializeModelClass(directXClass->d3dDevice, directXClass->d3dContext))
			return false;

	// Create Terrain Class
	terrainClass = new TerrainClass();
	if (terrainClass)
		if (!terrainClass->InitializeTerrain(directXClass->d3dDevice, directXClass->d3dContext))
			return false;

	// Create Billboards
	billboardClass = new BillboardClass();
	if (billboardClass)
		if (!billboardClass->InitializeBillboards(directXClass->d3dDevice, directXClass->d3dContext))
			return false;

	// Create dynamic Meshes
	dynamicModelClass = new DynamicModelClass();
	if (dynamicModelClass)
		if (!dynamicModelClass->InitializeDynamicModels(directXClass->d3dDevice, directXClass->d3dContext))
			return false;

	// Create a new shader class, if it fails return false
	shaderClass = new ShaderClass();
	if (shaderClass)
		if (!shaderClass->InitializeShaders(directXClass->d3dDevice, directXClass->d3dContext))
			return false;

	// Create light Class
	lightClass = new LightClass();
	if (lightClass)
		if (!lightClass->InitializeLightClass(directXClass->d3dDevice, directXClass->d3dContext, inputClass))
			return false;

	return true;
}

void GraphicsClass::ShutdownGraphics()
{
	if (directXClass)
		directXClass->ShutdownDirectX();

	if (shaderClass)
		shaderClass->ShutdownShaders();

	if (modelClass)
		modelClass->ShutdownModels();

	if (billboardClass)
		billboardClass->ShutdownModels();

	directXClass = 0;
	shaderClass = 0;
	modelClass = 0;
	billboardClass = 0;
}

void GraphicsClass::DrawGraphics()
{
	// Background Colour
	float colour[] = { 0.0f, 0.69f, 0.60f, 1.0f };

	// clear back buffer and apply new colour
	directXClass->d3dContext->ClearRenderTargetView(directXClass->d3dBackBuffer, colour);

	// Clear the depth buffer for current frame
	directXClass->d3dContext->ClearDepthStencilView(directXClass->depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	// Render Shaders
	shaderClass->RenderShaders(directXClass->d3dContext);

	// Render Terrain
	terrainClass->RenderTerrain(cameraClass->matWorld, cameraClass->matView, cameraClass->matProjection);

	// Render Models
	modelClass->RenderModels(cameraClass->matWorld, cameraClass->matView, cameraClass->matProjection);

	// Render Water
	dynamicModelClass->RenderModels(cameraClass->matWorld, cameraClass->matView, cameraClass->matProjection);

	// Render Billboards
	billboardClass->RenderModels(cameraClass->matWorld, cameraClass->matView, cameraClass->matProjection);

	// Render Lights
	lightClass->Renderlights(cameraClass->GetCameraPosition());

	// Update Screen
	directXClass->swapChain->Present(0, 0);
}

void GraphicsClass::Update(int fps, int mouseXPosition, int mouseYPosition)
{
	// Check if window is active before updating
	// updates go here
	if (GetActiveWindow() == windowHandle)
	{
		modelClass->UpdateModels(cameraClass->GetCameraPosition());
		dynamicModelClass->UpdateModels(cameraClass->GetCameraPosition());
		billboardClass->UpdateModels(cameraClass->GetCameraPosition());
		cameraClass->InitialiseCamera(windowHandle, inputClass);
		cameraClass->Update(fps, mouseXPosition, mouseYPosition);
		lightClass->UpdateLights();
	}
}
