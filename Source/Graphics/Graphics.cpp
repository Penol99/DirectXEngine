#define _USE_MATH_DEFINES
#include "Graphics.h"
#include <filesystem>
#include "DirectionalLight.h"
#include "../Entity Component System/Components/MaterialComponent.h"
#include "../Entity Component System/Components/BoxColliderComponent.h"

#define VSYNC_ENABLED true


// Global variables
bool gShowFBXWindow = true;
bool gShowTextureWindow = false;
std::string gFBXFilePath;
std::wstring gTextureFilePath;

bool Graphics::Init(HWND hwnd, int aWidth, int aHeight, Timer& aTimer)
{

	myTimer = &aTimer;
	myWidth = aWidth;
	myHeight = aHeight;
	myHandle = hwnd;
	if (!InitDirectX(hwnd))
	{
		return false;
	}
	if (!InitScene())
	{
		return false;
	}
	if (!InitGrid())
	{
		return false;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(myDevice.Get(), myDeviceContext.Get());
	ImGui::StyleColorsDark();

	return true;
}

void Graphics::Render(const int& aFPS, const float& aDeltaTime)
{
	ID3D11RenderTargetView* oldRenderTarget;
	ID3D11DepthStencilView* oldDepthStencilView;

	myDeviceContext->OMGetRenderTargets(1, &oldRenderTarget, &oldDepthStencilView);

	myDeviceContext->OMSetRenderTargets(1, myCameraRenderTargetView.GetAddressOf(), myDepthStencilView.Get());

	float backgroundColor[] = { 0.0f, 0.0f, 0.f , 1.0f };
	myDeviceContext->ClearRenderTargetView(myCameraRenderTargetView.Get(), backgroundColor);
	myDeviceContext->ClearDepthStencilView(myDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	RenderGrid();

	for (auto& model : myGameObjects)
	{
		model->Render();
	}

	myDeviceContext->OMSetRenderTargets(1, &oldRenderTarget, oldDepthStencilView);

	// Render ImGui here
	RenderImGui();

	mySwapChain->Present(VSYNC_ENABLED, NULL);

	if (oldRenderTarget != nullptr)
	{
		oldRenderTarget->Release();
	}
	if (oldDepthStencilView != nullptr)
	{
		oldDepthStencilView->Release();
	}
}

void Graphics::RenderImGui()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// DockSpaceOverViewport creates a dockspace which we can add windows to.
	ImGuiID dockspace_id = ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

	// SetNextWindowDockID sets the next window to dock into the specified dockspace.
	ImGui::SetNextWindowDockID(dockspace_id, ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(myWidth, myHeight), ImGuiCond_FirstUseEver);
	// Begin creating the window.
	ImGui::Begin("Camera Window");

	// Calculate the size of the window
	ImVec2 windowSize = ImGui::GetWindowSize();

	// Calculate the aspect ratio of the image
	float aspectRatio = (float)myWidth / (float)myHeight;

	// Calculate the size of the image
	float imageWidth, imageHeight;

	// Aspect ratio is width / height. If it's less than 1, the height is larger than the width.
	if (aspectRatio < 1.0f)
	{
		imageHeight = windowSize.y; // Take up the full window height
		imageWidth = imageHeight * aspectRatio; // Adjust width to maintain aspect ratio
	}
	else
	{
		imageWidth = windowSize.x; // Take up the full window width
		imageHeight = imageWidth / aspectRatio; // Adjust height to maintain aspect ratio
	}
	// Adjust the position of the image to center it in the window
	float imageX = (windowSize.x - imageWidth) / 2.0f;
	float imageY = (windowSize.y - imageHeight) / 2.0f;

	// Display the image
	ImGui::SetCursorPos(ImVec2(imageX, imageY));
	ImGui::Image(myCameraShaderResourceView.Get(), ImVec2(imageWidth, imageHeight));

	ImGui::End();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void Graphics::Resize(int aWidth, int aHeight)
{
	// Save the new width and height
	myWidth = aWidth;
	myHeight = aHeight;

	// Release current render target and depth/stencil view
	myRenderTargetView.Reset();
	myDepthStencilView.Reset();

	// Resize the swap chain
	HRESULT hr = mySwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "Failed to resize the swap chain.");
		return;
	}

	// Recreate the render target view
	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	hr = mySwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "GetBuffer Failed.");
		return;
	}

	hr = myDevice->CreateRenderTargetView(backBuffer.Get(), NULL, myRenderTargetView.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "Failed to create render target view.");
		return;
	}

	// Recreate the depth/stencil view
	D3D11_TEXTURE2D_DESC depthTextureDesc;
	depthTextureDesc.Width = myWidth;
	depthTextureDesc.Height = myHeight;
	depthTextureDesc.MipLevels = 1;
	depthTextureDesc.ArraySize = 1;
	depthTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthTextureDesc.SampleDesc.Count = 1;
	depthTextureDesc.SampleDesc.Quality = 0;
	depthTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	depthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthTextureDesc.CPUAccessFlags = 0;
	depthTextureDesc.MiscFlags = 0;
	
	hr = myDevice->CreateTexture2D(&depthTextureDesc, NULL, myDepthStencilBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "Failed to create depth stencil buffer.");
		return;
	}

	hr = myDevice->CreateDepthStencilView(myDepthStencilBuffer.Get(), NULL, myDepthStencilView.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "Failed to create depth stencil view.");
		return;
	}

	// Bind the render target view and depth/stencil view to the pipeline
	myDeviceContext->OMSetRenderTargets(1, myRenderTargetView.GetAddressOf(), myDepthStencilView.Get());
	myDeviceContext->OMSetDepthStencilState(myDepthStencilState.Get(), 0);

	// Set the viewport
	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<float>(myWidth);
	viewport.Height = static_cast<float>(myHeight);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	myDeviceContext->RSSetViewports(1, &viewport);

	// Recreate camera resources
	myCameraRenderTargetView.Reset();
	myCameraShaderResourceView.Reset();

	D3D11_TEXTURE2D_DESC camTextureDesc = {};
	camTextureDesc.Width = myWidth;
	camTextureDesc.Height = myHeight;
	camTextureDesc.MipLevels = 1;
	camTextureDesc.ArraySize = 1;
	camTextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	camTextureDesc.SampleDesc.Count = 1;
	camTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	camTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> offscreenTexture;
	hr = myDevice->CreateTexture2D(&camTextureDesc, nullptr, offscreenTexture.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "Failed recreating texture2D for camera");
		return;
	}

	D3D11_RENDER_TARGET_VIEW_DESC camRenderTargetViewDesc = {};
	camRenderTargetViewDesc.Format = camTextureDesc.Format;
	camRenderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	hr = myDevice->CreateRenderTargetView(offscreenTexture.Get(), &camRenderTargetViewDesc, myCameraRenderTargetView.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "Failed recreating render target view for camera");
		return;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC camShaderResourceViewDesc = {};
	camShaderResourceViewDesc.Format = camTextureDesc.Format;
	camShaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	camShaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	camShaderResourceViewDesc.Texture2D.MipLevels = -1;

	hr = myDevice->CreateShaderResourceView(offscreenTexture.Get(), &camShaderResourceViewDesc, myCameraShaderResourceView.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "Failed recreating shader resource view for camera");
		return;
	}

	// Release the offscreenTexture explicitly since it's no longer needed
	offscreenTexture.Reset();
}

bool Graphics::InitDirectX(HWND hwnd)
{
	RECT rect;
	GetClientRect(myHandle, &rect);
	myWidth = rect.right - rect.left;
	myHeight = rect.bottom - rect.top;

	if (!CreateSwapChain(hwnd))
	{
		return false;
	}

	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	HRESULT hr = mySwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));

	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "GetBuffer failed stupid.");
		return false;
	}
	hr = myDevice->CreateRenderTargetView(backBuffer.Get(), NULL, myRenderTargetView.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "failed creating render target view.");
		return false;
	}

	D3D11_TEXTURE2D_DESC depthTextureDesc;
	depthTextureDesc.Width = myWidth;
	depthTextureDesc.Height = myHeight;
	depthTextureDesc.MipLevels = 1;
	depthTextureDesc.ArraySize = 1;
	depthTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthTextureDesc.SampleDesc.Count = 1;
	depthTextureDesc.SampleDesc.Quality = 0;
	depthTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	depthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthTextureDesc.CPUAccessFlags = 0;
	depthTextureDesc.MiscFlags = 0;

	hr = myDevice->CreateTexture2D(&depthTextureDesc, NULL, myDepthStencilBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "failed creating depth stencil buffer.");
		return false;
	}
	hr = myDevice->CreateDepthStencilView(myDepthStencilBuffer.Get(), NULL, myDepthStencilView.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "failed creating depth stencil view.");
		return false;
	}
	myDeviceContext->OMSetRenderTargets(1, myRenderTargetView.GetAddressOf(), myDepthStencilView.Get());
	myDeviceContext->OMSetDepthStencilState(myDepthStencilState.Get(), 0);
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

	hr = myDevice->CreateDepthStencilState(&depthStencilDesc, myDepthStencilState.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "failed creating depth state.");
		return false;
	}


	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = myWidth;
	viewport.Height = myHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	myDeviceContext->RSSetViewports(1, &viewport);

	//Rasterizer state 
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;

	hr = myDevice->CreateRasterizerState(&rasterizerDesc, myRasterizerState.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "failed to create rasterizer state.");
		return false;
	}

	// TODO: Create a font manager;
	mySpriteBatch = std::make_unique<SpriteBatch>(myDeviceContext.Get());
	mySpriteFont = std::make_unique<SpriteFont>(myDevice.Get(), L"../bin/Assets/Fonts/Arial_DX.spriteFont");

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = myDevice->CreateSamplerState(&samplerDesc, mySamplerState.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "failed creating sampler state.");
		return false;
	}

	
	D3D11_TEXTURE2D_DESC camTextureDesc = {};
	D3D11_RENDER_TARGET_VIEW_DESC camRenderTargetViewDesc = {};
	D3D11_SHADER_RESOURCE_VIEW_DESC camShaderResourceViewDesc = {};


	camTextureDesc.Width = myWidth;
	camTextureDesc.Height = myHeight;
	camTextureDesc.MipLevels = 1;
	camTextureDesc.ArraySize = 1;
	camTextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	camTextureDesc.SampleDesc.Count = 1;
	camTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	camTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	camTextureDesc.CPUAccessFlags = 0;
	camTextureDesc.MiscFlags = 0;

	ID3D11Texture2D* offscreenTexture;
	myDevice->CreateTexture2D(&camTextureDesc, NULL, &offscreenTexture);

	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "Failed creating texture2D for camera");
		return false;
	}
	camRenderTargetViewDesc.Format = camTextureDesc.Format;
	camRenderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	camRenderTargetViewDesc.Texture2D.MipSlice = 0;

	hr = myDevice->CreateRenderTargetView(offscreenTexture, &camRenderTargetViewDesc, &myCameraRenderTargetView);
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "Failed creating render target view for camera");
		return false;
	}

	camShaderResourceViewDesc.Format = camTextureDesc.Format;
	camShaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	camShaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	camShaderResourceViewDesc.Texture2D.MipLevels = 1;

	hr = myDevice->CreateShaderResourceView(offscreenTexture, &camShaderResourceViewDesc, &myCameraShaderResourceView);
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "Failed creating shader resource view for camera");
		return false;
	}
	offscreenTexture->Release();




	return true;
}

bool Graphics::InitScene()
{

	myCamera.SetPosition(0.0f, 10.0f, -10.0f);
	myCamera.SetProjectionValues(70.f, static_cast<float>(myWidth) / static_cast<float>(myHeight), 0.1f, 1000.f);

	GameObject* man = new GameObject();
	man->Init(myTimer, &myCamera, myDevice, myDeviceContext);

	MaterialComponent* material = man->AddComponent<MaterialComponent>();
	material->Init(L"../bin/shaders/PBRVertexShader.cso", L"../bin/shaders/PBRPixelShader.cso");
	material->SetTexture(L"../bin/assets/textures/man.jpg");
	material->SetReflectionTexture(L"../bin/assets/textures/reflection.jpg");

	ModelComponent* model = man->AddComponent<ModelComponent>();
	model->Init("../bin/assets/meshes/other/man.fbx");

	BoxColliderComponent* boxCollider = man->AddComponent<BoxColliderComponent>();
	boxCollider->SetExtents(XMFLOAT3(4, 40, 4));





	myGameObjects.push_back(man);

	return true;
}

bool Graphics::CreateSwapChain(HWND hwnd)
{

	std::vector<AdapterData> adapters = AdapterReader::GetAdapters();
	if (adapters.size() < 1)
	{
		ErrorLog::Log("lmao no IDXGI adapters found.");
		return false;
	}

	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferDesc.Width = myWidth;
	scd.BufferDesc.Height = myHeight;
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
	hr = D3D11CreateDeviceAndSwapChain(adapters[0].myAdapter, // GPU, IDXGI Adapter
		D3D_DRIVER_TYPE_UNKNOWN,
		NULL, // SOFWARE DRIVER TYPE
		NULL, // FLAGS FOR RUNTIME LAYERS
		NULL, // FEATURE LEVELS ARRAY
		0, //# OF FEATURE LEVELS IN ARRAY
		D3D11_SDK_VERSION,
		&scd, // SWAPCHAIN DESCRIPTION
		mySwapChain.GetAddressOf(),
		myDevice.GetAddressOf(),
		NULL, // SUPPORTED FEATURE LEVEL
		myDeviceContext.GetAddressOf());

	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "failed creating device and swapchain, damn.");
		return false;
	}
	return true;
}

// Imgui window for selecting FBX files.
void Graphics::ShowFBXWindow(ImGuiWindowFlags& someFlags)
{
	ImGui::Begin("SELECT AN FBX", nullptr, someFlags);

	std::string fbxDirectoryPath = "../bin/Assets/Meshes/";

	std::string previousLabel;
	for (const auto& entry : std::filesystem::recursive_directory_iterator(fbxDirectoryPath))
	{
		const std::filesystem::path& filePath = entry.path();

		if (std::filesystem::is_directory(filePath))
		{
			std::string directoryName = filePath.filename().string();
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 50, 205, 255));
			ImGui::Text(" ");
			ImGui::Text(directoryName.c_str());
			ImGui::PopStyleColor();

		}
		else if (std::filesystem::is_regular_file(filePath) && filePath.extension() == ".fbx")
		{
			std::string fileName = filePath.filename().string();
			if (ImGui::Selectable(fileName.c_str()))
			{
				gFBXFilePath = filePath.string();
				gShowFBXWindow = false;
				gShowTextureWindow = true;
			}
		}
	}

}

// ImGui window for selecting texture file
void Graphics::ShowTextureWindow(ImGuiWindowFlags& someFlags)
{
	ImGui::Begin("SELECT A TEXTURE", nullptr, someFlags);

	std::string textureDirectoryPath = "../bin/Assets/Textures";

	std::vector<std::string> supportedExtensions = { ".png", ".dds", ".jpg" };

	for (const auto& entry : std::filesystem::directory_iterator(textureDirectoryPath))
	{
		const std::filesystem::path& filePath = entry.path();

		if (std::filesystem::is_regular_file(filePath) && std::find(supportedExtensions.begin(), supportedExtensions.end(), filePath.extension()) != supportedExtensions.end())
		{
			std::string fileName = filePath.filename().string();
			if (ImGui::Selectable(fileName.c_str()))
			{
				gTextureFilePath = filePath.wstring();
				gShowTextureWindow = false;
				std::wstring standardVertexShader = L"../bin/PBRVertexShader.cso";
				std::wstring standardPixelShader = L"../bin/PBRPixelShader.cso";
				//LoadFBX(gFBXFilePath, gTextureFilePath, standardVertexShader, standardPixelShader);
				gShowFBXWindow = true;
			}
		}
	}

}

bool Graphics::InitGrid()
{
	const float numGridCellsX = 200;
	const float numGridCellsY = 200;
	const float gridSize = 10.f;
	const float gridWidth = gridSize * numGridCellsX; // Total width of the grid
	const float gridHeight = gridSize * numGridCellsY; // Total height of the grid

	// Set up the grid vertices
	std::vector<Vertex> gridVertices;

	// Horizontal grid lines
	for (int y = 0; y <= numGridCellsY; ++y)
	{
		float zPos = y * gridSize - gridHeight * 0.5f;

		gridVertices.push_back(Vertex(-gridWidth * 0.5f, 0.0f, zPos, 0.0f, zPos / gridHeight, 0, 0, 0));

		gridVertices.push_back(Vertex(gridWidth * 0.5f, 0.0f, zPos, 1.0f, zPos / gridHeight, 0, 0, 0));
	}

	// Vertical grid lines
	for (int x = 0; x <= numGridCellsX; ++x)
	{
		float xPos = x * gridSize - gridWidth * 0.5f;

		gridVertices.push_back(Vertex(xPos, 0.0f, -gridHeight * 0.5f, xPos / gridWidth, 0.0f, 0, 0, 0));

		gridVertices.push_back(Vertex(xPos, 0.0f, gridHeight * 0.5f, xPos / gridWidth, 1.0f, 0, 0, 0));
	}

	HRESULT hr = myGridVertexBuffer.Init(myDevice.Get(), &gridVertices[0], static_cast<UINT>(gridVertices.size()));

	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "Failed initializing grid vertex buffer.");
		return false;

	}

	std::vector<DWORD> gridIndices;

	for (UINT i = 0; i < gridVertices.size(); i += 2)
	{
		gridIndices.push_back(i);
		gridIndices.push_back(i + 1);
	}

	hr = myGridIndexBuffer.Init(myDevice.Get(), &gridIndices[0], static_cast<UINT>(gridIndices.size()));
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "Failed initializing grid indices buffer.");
		return false;
	}

	hr = myGridConstantBuffer.Init(myDevice.Get(), myDeviceContext.Get());
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "Failed initializing grid constant buffer.");
		return false;
	}

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION",0, DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0}
	};

	UINT numElements = ARRAYSIZE(layout);

	if (!myLineVertexShader.Init(myDevice, L"../bin/shaders/LineVertexShader.cso", layout, numElements))
	{
		return false;
	}

	if (!myLinePixelShader.Init(myDevice, L"../bin/shaders/LinePixelShader.cso"))
	{
		return false;
	}

	return true;
}

void Graphics::RenderGrid()
{
	UINT stride = myGridVertexBuffer.GetStride();
	UINT offset = 0;


	myGridConstantBuffer.myData.worldMatrix = myCamera.GetViewMatrix() * myCamera.GetProjectionMatrix();
	myGridConstantBuffer.myData.worldMatrix = XMMatrixTranspose(myGridConstantBuffer.myData.worldMatrix);
	myGridConstantBuffer.myData.modelPosition = XMFLOAT3(0, 0, 0);
	myGridConstantBuffer.myData.modelRotation = XMFLOAT3(0, 0, 0);
	myDeviceContext->UpdateSubresource(myGridConstantBuffer.Get(), 0, nullptr, &myGridConstantBuffer.myData, 0, 0);
	myGridConstantBuffer.ApplyChanges();
	myDeviceContext->VSSetConstantBuffers(0, 1, myGridConstantBuffer.GetAddressOf());

	myDeviceContext->IASetInputLayout(myLineVertexShader.GetInputLayout());
	myDeviceContext->VSSetShader(myLineVertexShader.GetShader(), NULL, 0);
	myDeviceContext->PSSetShader(myLinePixelShader.GetShader(), NULL, 0);

	myDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	myDeviceContext->IASetVertexBuffers(0, 1, myGridVertexBuffer.GetAddressOf(), &stride, &offset);
	myDeviceContext->IASetIndexBuffer(myGridIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	myDeviceContext->DrawIndexed(myGridIndexBuffer.GetBufferSize(), 0, 0);
}


