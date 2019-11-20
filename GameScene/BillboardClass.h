#pragma once
#include "ModelClass.h"
class BillboardClass : public ModelClass
{
public:
	BillboardClass();
	bool InitializeBillboards(ID3D11Device *d3dDevice, ID3D11DeviceContext * d3dContext);
	void UpdateModels(XMFLOAT3 cameraPosition) override;
	void RenderModels(XMMATRIX matWorld, XMMATRIX matView, XMMATRIX matProjection) override; // Fixed the billboard clipping

private:
	bool InitializeObjects() override;
	bool InitializeInstancedObjects() override;
	XMFLOAT3 cameraPosition;
};

