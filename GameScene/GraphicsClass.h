#include "DirectXClass.h"
#include "ShaderClass.h"
#include "ModelClass.h"
#include "CameraClass.h"
#include "TerrainClass.h"
#include "BillboardClass.h"
#include "DynamicModelClass.h"
#include "LightClass.h"

class GraphicsClass
{
public:
	GraphicsClass();
	bool InitializeGraphics(HWND windowHandle, class InputClass *inputClass);
	void ShutdownGraphics();
	void DrawGraphics();
	void Update(int fps, int mouseXPosition, int mouseYPosition);

private:
	DirectXClass *directXClass = nullptr;
	ShaderClass *shaderClass = nullptr;
	ModelClass *modelClass = nullptr;
	CameraClass *cameraClass = nullptr;
	InputClass *inputClass = nullptr;
	TerrainClass *terrainClass = nullptr;
	BillboardClass *billboardClass = nullptr;
	DynamicModelClass *dynamicModelClass = nullptr;
	LightClass *lightClass = nullptr;

	HWND windowHandle = nullptr;
};

