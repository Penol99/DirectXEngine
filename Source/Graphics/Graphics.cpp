#include "Graphics.h"

#define VSYNC_ENABLED false

bool Graphics::Initialize(HWND hwnd, int aWidth, int aHeight)
{
	if (!InitializeDirectX(hwnd, aWidth, aHeight))
	{
		return false;
	}
	if (!InitializeShaders())
	{
		return false;
	}
	return true;
}

void Graphics::RenderFrame()
{
	float bgColor[] = {0.3f, 0.0f,0.5f,1.0f};
	mDeviceContext->ClearRenderTargetView(mRenderTargetView.Get(), bgColor);
	mSwapChain->Present(VSYNC_ENABLED, NULL);
}

bool Graphics::InitializeDirectX(HWND hwnd, int aWidth, int aHeight)
{
	std::vector<AdapterData> adapters = AdapterReader::GetAdapters();

	if (adapters.size() < 1)
	{
		ErrorLogger::Log("lmao no IDXGI adapters found.");
		return false;
	}

	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferDesc.Width = aWidth;
	scd.BufferDesc.Height = aHeight;
	scd.BufferDesc.RefreshRate.Numerator = 144; // TODO: Fix so that it gets the refresh rate from the monitor instead.
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	scd.SampleDesc.Count = 1; // Multisamples per pixel
	scd.SampleDesc.Quality = 0;

	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.BufferCount = 1;
	scd.OutputWindow = hwnd;
	scd.Windowed = true;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // Allows toggling between windowed and fullscreen.

	HRESULT hr;
	hr = D3D11CreateDeviceAndSwapChain(adapters[0].mAdapter, // GPU, IDXGI Adapter
										D3D_DRIVER_TYPE_UNKNOWN,
										NULL, // SOFWARE DRIVER TYPE
										NULL, // FLAGS FOR RUNTIME LAYERS
										NULL, // FEATURE LEVELS ARRAY
										0, //# OF FEATURE LEVELS IN ARRAY
										D3D11_SDK_VERSION,
										&scd, // SWAPCHAIN DESCRIPTION
										mSwapChain.GetAddressOf(),
										mDevice.GetAddressOf(),
										NULL, // SUPPORTED FEATURE LEVEL
										mDeviceContext.GetAddressOf());

	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "fucked up creating device and swapchain, damn.");
		return false;
	}

	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));

	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "GetBuffer failed stupid.");
		return false;
	}
	hr = mDevice->CreateRenderTargetView(backBuffer.Get(), NULL, mRenderTargetView.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "fucked up creating render target view.");
		return false;
	}
	mDeviceContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), NULL);
	return true;
}

bool Graphics::InitializeShaders()
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION",0, DXGI_FORMAT_R32G32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0}
	};

	UINT numElements = ARRAYSIZE(layout);

	HRESULT hr = mDevice->CreateInputLayout(layout, numElements, mVertexShaderBuffer->GetBufferPointer(), mVertexShaderBuffer->GetBufferSize(), mInputLayout.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "fucked up creating input layout, rip.");
		return false;
	}

	
	return true;
}
