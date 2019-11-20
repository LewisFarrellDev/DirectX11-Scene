#include <d3d11.h>
class ShaderClass
{
public:
	ShaderClass();
	bool InitializeShaders(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dContext);
	void ShutdownShaders();
	void RenderShaders(ID3D11DeviceContext *d3dDevice);

private:
	ID3D11VertexShader *vertexShader = NULL;
	ID3D11PixelShader *pixelShader = NULL;
	ID3D11InputLayout *vertexLayout = NULL;
	ID3D11ShaderResourceView *tex = NULL;
	ID3D11SamplerState *sampler = NULL;
};

