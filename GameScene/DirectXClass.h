#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class DirectXClass
{
public:
	DirectXClass();
	bool InitializeDirectX(HWND windowHandle);
	void ShutdownDirectX();

	ID3D11Device *d3dDevice;
	ID3D11DeviceContext *d3dContext;
	ID3D11RenderTargetView *d3dBackBuffer;
	IDXGISwapChain *swapChain;
	ID3D11Texture2D *depthStencilBuffer;
	ID3D11DepthStencilView *depthStencilView;
	ID3D11BlendState *d3dBlendState;

	
private:	
	
};

