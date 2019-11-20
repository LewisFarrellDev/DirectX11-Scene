#include <DirectXMath.h>
#include <Windows.h>

using namespace DirectX;

class CameraClass
{
public:
	CameraClass();

	void UpdateCameraPositionRotation();
	void Update(float fps, int mouseXPosition, int mouseYPosition);

	bool InitialiseCamera(HWND windowHandle, class InputClass *inputClass);
	XMFLOAT3 GetCameraPosition() { return position; }

	XMMATRIX matView;
	XMMATRIX matProjection;
	XMMATRIX matWorld;

private:
	class InputClass *inputClass;

	float drawDistance;

	// Camera Position
	XMFLOAT3 position = XMFLOAT3(0, 0, 0);

	// calculation Variables
	float tempPositionX;
	float tempPositionY;
	float tempPositionZ;

	// Rotation of the camera
	float rotationX;
	float rotationY;

	float cameraSpeed;
	float rotationSpeed;

	// Mouse Location
	int mouseXPosition;
	int mouseYPosition;

	// Client Middle Points
	int clientMidX;
	int clientMidY;

	// Window Position
	int windowMidX, windowMidY;
	int windowLeft, windowTop;

	// rot factor
	float rotFactorY = 0.0f;
	float rotFactorX = 0.0f;

	XMVECTOR camTarget;
	XMVECTOR camUp;
	XMVECTOR camPos;	
};

