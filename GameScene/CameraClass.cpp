#include "CameraClass.h"
#include "InputClass.h"

CameraClass::CameraClass()
{
	drawDistance = 3000.f;

	position.x = 0;
	position.y = 10;
	position.z = -20;

	rotationX = 0.f;
	rotationY = 0.f;

	cameraSpeed = 50;
	rotationSpeed = 0.3f;

	camTarget = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	camUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
}

void CameraClass::UpdateCameraPositionRotation()
{
	int deltaX = clientMidX - mouseXPosition;
	rotationX -= deltaX * rotFactorY * rotationSpeed;

	int deltaY = clientMidY - mouseYPosition;
	rotationY -= deltaY * rotFactorX * (rotationSpeed / 1.5f);

	SetCursorPos(windowLeft + windowMidX, windowTop + windowMidY);

	XMMATRIX camMove = XMMatrixTranslation(tempPositionX, tempPositionY, tempPositionZ) *
		XMMatrixRotationRollPitchYaw(rotationY, rotationX, 0.0f);

	XMVECTOR scale, rot, trans;
	XMMatrixDecompose(&scale, &rot, &trans, camMove);

	position.x += XMVectorGetX(trans);
	position.y += XMVectorGetY(trans);
	position.z += XMVectorGetZ(trans);

	camPos = XMVectorSet(position.x, position.y, position.z, 0.0f);
	

	XMMATRIX camDist = XMMatrixTranslation(0.0f, 0.0f, 10.0f) *
		XMMatrixRotationRollPitchYaw(rotationY, rotationX, 0.0f);

	XMMatrixDecompose(&scale, &rot, &trans, camDist);

	camTarget = XMVectorSet(position.x + XMVectorGetX(trans), position.y + XMVectorGetY(trans), position.z + XMVectorGetZ(trans), 0.f);
	matView = XMMatrixLookAtLH(camPos, camTarget, camUp);

}

void CameraClass::Update(float fps, int mouseXPosition, int mouseYPosition)
{
	tempPositionX = 0;
	tempPositionY = 0;
	tempPositionZ = 0;
	this->mouseXPosition = mouseXPosition;
	this->mouseYPosition = mouseYPosition;
	cameraSpeed = 50;

	if (fps != 0)
	{
		if (inputClass->IsKeyDown(VK_SHIFT))
			cameraSpeed = 150;

		if (inputClass->IsKeyDown('W'))
			tempPositionZ = cameraSpeed * (1 / (float)fps);

		if (inputClass->IsKeyDown('S'))
			tempPositionZ = -cameraSpeed * (1 / (float)fps);

		if (inputClass->IsKeyDown('A'))
			tempPositionX = -cameraSpeed * (1 / (float)fps);

		if (inputClass->IsKeyDown('D'))
			tempPositionX = cameraSpeed * (1 / (float)fps);

	}

	UpdateCameraPositionRotation();
}

bool CameraClass::InitialiseCamera(HWND windowHandle, InputClass *inputClass)
{
	this->inputClass = inputClass;

	// Get the current size of the window and store the height and width
	RECT rectClient;
	GetClientRect(windowHandle, &rectClient);

	LONG width = rectClient.right - rectClient.left;
	LONG height = rectClient.bottom - rectClient.top - GetSystemMetrics(SM_CYCAPTION);

	RECT rectWindow;
	GetWindowRect(windowHandle, &rectWindow);

	// top left coordinate for the cursor
	windowTop = rectWindow.top;
	windowLeft = rectWindow.left;

	// Midpoint of the window
	windowMidX = (rectWindow.right - rectWindow.left) / 2;
	windowMidY = (rectWindow.bottom - rectWindow.top) / 2;

	// mid point of the client area
	clientMidX = width / 2;
	clientMidY = height / 2;

	// Set mouse Position
	mouseXPosition = clientMidX;
	mouseYPosition = clientMidY;

	// set cursor pos
	SetCursorPos(windowLeft + windowMidX, windowTop + windowMidY);

	// Calculation rotation
	rotFactorY = XM_PIDIV2 / width;
	rotFactorX = XM_PIDIV2 / height;

	// View Matrix
	camPos = XMVectorSet(position.x, position.y, position.z, 0.f);
	matView = XMMatrixLookAtLH(camPos, camTarget, camUp);

	matProjection = XMMatrixPerspectiveFovLH(XM_PIDIV4, (width / (float)height), 1.0f, drawDistance);

	return true;
}
