#pragma once
#include "ModelClass.h"
class DynamicModelClass : public ModelClass
{
public:
	DynamicModelClass();
	bool InitializeDynamicModels(ID3D11Device *d3dDevice, ID3D11DeviceContext * d3dContext);
	void UpdateModels(XMFLOAT3 cameraPosition) override;

private:
	bool InitializeObjects() override;
	bool InitializeInstancedObjects() override;
};

