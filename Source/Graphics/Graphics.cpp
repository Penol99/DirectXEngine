#include "Graphics.h"
//#include <fbxsdk.h>
//#include <fbxsdk/core/math/fbxmath.h>
//#include <fbxsdk/core/math/fbxquaternion.h>


#define VSYNC_ENABLED true


// Global variables
bool gShowFBXWindow = true;
bool gShowTextureWindow = false;
std::string gFBXFilePath;
std::wstring gTextureFilePath;

bool Graphics::Init(HWND hwnd, int aWidth, int aHeight)
{
	mWidth = aWidth;
	mHeight = aHeight;
	if (!InitDirectX(hwnd))
	{
		return false;
	}
	if (!InitShaders())
	{
		return false;
	}
	if (!InitScene())
	{
		return false;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(mDevice.Get(), mDeviceContext.Get());
	ImGui::StyleColorsDark();

	return true;
}

void Graphics::Render(const int& aFPS, const float& aDeltaTime)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	float backgroundColor[] = { 0.0f, 0.0f, 0.0f,1.0f };
	mDeviceContext->ClearRenderTargetView(mRenderTargetView.Get(), backgroundColor);
	mDeviceContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//mDeviceContext->IASetInputLayout(mVertexShader.GetInputLayout());
	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mDeviceContext->RSSetState(mRasterizerState.Get());
	mDeviceContext->OMSetDepthStencilState(mDepthStencilState.Get(), 0);
	mDeviceContext->PSSetSamplers(0, 1, mSamplerState.GetAddressOf());


	//myPlayer.Render(mDeviceContext.Get());
	//myScooter.Render(mDeviceContext.Get());

	//for (auto& m : myModels)
	//{
	//	m.Render(mDeviceContext.Get());
	//}
	
	for (auto& model : myModels)
	{
		// Create a unique window for each model
		ImGui::Begin(model.GetName().c_str());

		// Display position and rotation fields using ImGui input functions
		ImGui::Text("Position");
		ImGui::DragFloat("X##Pos", &model.mPosition.x, 0.1f);
		ImGui::DragFloat("Y##Pos", &model.mPosition.y, 0.1f);
		ImGui::DragFloat("Z##Pos", &model.mPosition.z, 0.1f);

		ImGui::Text("Rotation");
		ImGui::DragFloat("X##Rot", &model.mRotationAngles.x, 0.1f);
		ImGui::DragFloat("Y##Rot", &model.mRotationAngles.y, 0.1f);
		ImGui::DragFloat("Z##Rot", &model.mRotationAngles.z, 0.1f);

		ImGui::End();

		// Render the model using the updated position and rotation
		model.Render(mDeviceContext.Get());
	}

	mSpriteBatch->Begin();
	std::wstring fpsCounter = L"FPS: ";
	fpsCounter += std::to_wstring(aFPS);
	mSpriteFont->DrawString(mSpriteBatch.get(), fpsCounter.c_str(), XMFLOAT2(0, 0), Colors::White, 0.0f, XMFLOAT2(0, 0), XMFLOAT2(1.0f, 1.0f));
	mSpriteBatch->End();
	
	
	ImGui::Begin("FBX", &gShowFBXWindow, ImGuiWindowFlags_AlwaysAutoResize);

	if (gShowFBXWindow)
	{
		ShowFBXWindow();
	}
	else if (gShowTextureWindow)
	{
		ShowTextureWindow();
	}
	else
	{
	}
	ImGui::End();

	// Rendering
	// Added line to end the "Files" window
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());




	mSwapChain->Present(VSYNC_ENABLED, NULL);
}

bool Graphics::InitDirectX(HWND hwnd)
{
	std::vector<AdapterData> adapters = AdapterReader::GetAdapters();

	if (adapters.size() < 1)
	{
		ErrorLog::Log("lmao no IDXGI adapters found.");
		return false;
	}

	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferDesc.Width = mWidth;
	scd.BufferDesc.Height = mHeight;
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
		ErrorLog::Log(hr, "fucked up creating device and swapchain, damn.");
		return false;
	}

	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));

	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "GetBuffer failed stupid.");
		return false;
	}
	hr = mDevice->CreateRenderTargetView(backBuffer.Get(), NULL, mRenderTargetView.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "fucked up creating render target view.");
		return false;
	}

	D3D11_TEXTURE2D_DESC depthTextureDesc;
	depthTextureDesc.Width = mWidth;
	depthTextureDesc.Height = mHeight;
	depthTextureDesc.MipLevels = 1;
	depthTextureDesc.ArraySize = 1;
	depthTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthTextureDesc.SampleDesc.Count = 1;
	depthTextureDesc.SampleDesc.Quality = 0;
	depthTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	depthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthTextureDesc.CPUAccessFlags = 0;
	depthTextureDesc.MiscFlags = 0;

	hr = mDevice->CreateTexture2D(&depthTextureDesc, NULL, mDepthStencilBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "fucked up creating depth stencil buffer.");
		return false;
	}
	hr = mDevice->CreateDepthStencilView(mDepthStencilBuffer.Get(), NULL, mDepthStencilView.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "fucked up creating depth stencil view.");
		return false;
	}
	mDeviceContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

	hr = mDevice->CreateDepthStencilState(&depthStencilDesc, mDepthStencilState.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "fucked up creating depth state.");
		return false;
	}


	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = mWidth;
	viewport.Height = mHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	mDeviceContext->RSSetViewports(1, &viewport);

	//Rasterizer state 
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;

	hr = mDevice->CreateRasterizerState(&rasterizerDesc, mRasterizerState.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "dick and balls failed to create rasterizer state.");
		return false;
	}

	// TODO: Create a font manager;
	mSpriteBatch = std::make_unique<SpriteBatch>(mDeviceContext.Get());
	mSpriteFont = std::make_unique<SpriteFont>(mDevice.Get(), L"../Assets/Fonts/Arial_DX.spriteFont");

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = mDevice->CreateSamplerState(&samplerDesc, mSamplerState.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "fucked up creating sampler state.");
		return false;
	}

	return true;
}

bool Graphics::InitShaders()
{

	return true;
}

bool Graphics::InitScene()
{

	//myPlayer.Init(mDevice, mDeviceContext,"../Assets/Meshes/SK_Player.fbx", L"../Assets/Textures/SK_Player_c.dds", mCamera);
	//myScooter.Init(mDevice, mDeviceContext,"../Assets/Meshes/Scooter.fbx", L"../Assets/Textures/Scooter.png", mCamera);


	mCamera.SetPosition(0.0f, 0.0f, -2.0f);
	mCamera.SetProjectionValues(90.f, static_cast<float>(mWidth) / static_cast<float>(mHeight), 0.1f, 1000.f);
	return true;
}

void Graphics::LoadFBX(std::string& filePath, std::wstring& aTexturePath)
{
	Model model;
	model.Init(mDevice, mDeviceContext, filePath, aTexturePath, mCamera);
	myModels.push_back(model);
}


void Graphics::ShowFBXWindow()
{
	std::string fbxDirectoryPath = "../Assets/Meshes/";

	// Iterate over the files in the directory
	WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	std::string fbxFilePattern = fbxDirectoryPath + "/*.fbx";
	hFind = FindFirstFile(StringConverter::StringToWide(fbxFilePattern).c_str(), &ffd);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			// Display each FBX file as a selectable item
			if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				int bufferSize = WideCharToMultiByte(CP_UTF8, 0, ffd.cFileName, -1, nullptr, 0, nullptr, nullptr);
				std::string fileName(bufferSize, 0);
				WideCharToMultiByte(CP_UTF8, 0, ffd.cFileName, -1, &fileName[0], bufferSize, nullptr, nullptr);
				if (ImGui::Selectable(fileName.c_str()))
				{
					gFBXFilePath = fbxDirectoryPath + "/" + fileName;
					gShowFBXWindow = false;
					gShowTextureWindow = true;
				}
			}
		} while (FindNextFile(hFind, &ffd) != 0);
		FindClose(hFind);
	}
}

// ImGui window for selecting texture file
void Graphics::ShowTextureWindow()
{
	ImGui::Begin("Texture Selection", &gShowTextureWindow, ImGuiWindowFlags_AlwaysAutoResize);

	std::wstring textureDirectoryPath = L"../Assets/Textures";

	// Specify the file extensions you want to include
	std::vector<std::wstring> supportedExtensions = { L".png", L".dds", L".jpg" };

	// Iterate over the files in the directory
	WIN32_FIND_DATAW ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	for (const auto& extension : supportedExtensions)
	{
		std::wstring textureFilePattern = textureDirectoryPath + L"/*" + extension;
		hFind = FindFirstFileW(textureFilePattern.c_str(), &ffd);
		if (hFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				// Display each texture file as a selectable item
				if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				{
					int bufferSize = WideCharToMultiByte(CP_UTF8, 0, ffd.cFileName, -1, nullptr, 0, nullptr, nullptr);
					std::string fileName(bufferSize, 0);
					WideCharToMultiByte(CP_UTF8, 0, ffd.cFileName, -1, &fileName[0], bufferSize, nullptr, nullptr);
					if (ImGui::Selectable(fileName.c_str()))
					{
						gTextureFilePath = textureDirectoryPath + L"/" + ffd.cFileName;
						gShowTextureWindow = false;
						LoadFBX(gFBXFilePath, gTextureFilePath);
						gShowFBXWindow = true;
					}
				}
			} while (FindNextFileW(hFind, &ffd) != 0);

			FindClose(hFind);
		}
	}

	ImGui::End();
}


