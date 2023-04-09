#pragma once
#include "AdapterReader.h"
using namespace Microsoft::WRL;
class Graphics
{
public:
	bool Initialize(HWND hwnd, int aWidth, int aHeight);
	void RenderFrame();
private:
	bool InitializeDirectX(HWND hwnd,int aWidth,int aHeight);
	bool InitializeShaders();
	ComPtr<ID3D11Device> mDevice;
	ComPtr<ID3D11DeviceContext> mDeviceContext;
	ComPtr<IDXGISwapChain> mSwapChain;
	ComPtr<ID3D11RenderTargetView> mRenderTargetView;
	ComPtr<ID3D11InputLayout> mInputLayout;
	ComPtr<ID3D10Blob> mVertexShaderBuffer;
};
