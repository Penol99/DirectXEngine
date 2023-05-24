#define _USE_MATH_DEFINES
#include "Graphics.h"
#include <filesystem>
#include "DirectionalLight.h"


#define VSYNC_ENABLED true


// Global variables
bool gShowFBXWindow = true;
bool gShowTextureWindow = false;
std::string gFBXFilePath;
std::wstring gTextureFilePath;

bool Graphics::Init(HWND hwnd, int aWidth, int aHeight,Timer& aTimer)
{

	myTimer = &aTimer;
	myWidth = aWidth;
	myHeight = aHeight;
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
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(myDevice.Get(), myDeviceContext.Get());
	ImGui::StyleColorsDark();

	return true;
}

void Graphics::Render(const int& aFPS, const float& aDeltaTime)
{
	//float backgroundColor[] = { 0.0f, 50.0f, 200.0f,1.0f };
	float backgroundColor[] = { 0.0f, 0.0f, 0.f , 1.0f };
	myDeviceContext->ClearRenderTargetView(myRenderTargetView.Get(), backgroundColor);
	myDeviceContext->ClearDepthStencilView(myDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	RenderGrid();



	myDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	myDeviceContext->RSSetState(myRasterizerState.Get());
	myDeviceContext->OMSetDepthStencilState(myDepthStencilState.Get(), 0);
	myDeviceContext->PSSetSamplers(0, 1, mySamplerState.GetAddressOf());



	//myPBRPixelShader.myData.direction = XMFLOAT3(0.5f, 0.5f, 0.5f);
	//myPBRPixelShader.myData.ambientColor = DirectionalLight::GetInstance()->myAmbientColor;
	//myPBRPixelShader.myData.diffuseColor = DirectionalLight::GetInstance()->myDiffuseColor;

	/*myPBRPixelShader.ApplyChanges();
	myDeviceContext->PSSetConstantBuffers(2, 1, myPBRPixelShader.GetAddressOf());*/





	for (auto& model : myModels)
	{

		
		model.Render(myDeviceContext.Get(), *myTimer);
	}


	const float fpsYOffset = 20;
	mySpriteBatch->Begin();
	std::wstring fpsCounter = L"FPS: ";
	fpsCounter += std::to_wstring(aFPS);
	mySpriteFont->DrawString(mySpriteBatch.get(), fpsCounter.c_str(), XMFLOAT2(0, myHeight - fpsYOffset), Colors::White, 0.0f, XMFLOAT2(0, 0), XMFLOAT2(1.0f, 1.0f));
	mySpriteBatch->End();

	
	RenderImGui();  
	mySwapChain->Present(VSYNC_ENABLED, NULL);
}

void Graphics::RenderImGui()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking;
	ImGuiViewport* viewport = ImGui::GetMainViewport();

	windowFlags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove;
	windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	const float itemWidth = 65;
	int modelIndex = 0;
	int previousImGuiWindowHeight = 0;
	const int myWidthOffset = 240;
	const int myHeightOffset = 0;
	ImGui::Begin("Controls:");
	ImGui::SetWindowSize(ImVec2(0, 0));
	ImGui::Text("HOLD DOWN RIGHT CLICK TO MOVE AND ROTATE CAMERA");
	ImGui::Text("WASD - Camera Move");
	ImGui::Text("SPACE - Move up");
	ImGui::Text("LCTRL - Move Down");
	ImGui::Text("LSHIFT - Speed up");
	ImGui::SetWindowPos(ImVec2((myWidth / 2) - ImGui::GetWindowSize().x / 2, 0));

	ImGui::End();
	ImGui::GetWindowSize();
	ImGui::SetNextWindowSize(ImVec2(0, 0));
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	ImGui::Begin("Directional Light", nullptr, windowFlags);
	previousImGuiWindowHeight = ImGui::GetWindowSize().y;
	ImGui::PopStyleVar(2);
	ImGui::PushItemWidth(itemWidth);

	ImGui::Text("Rotation");
	DirectionalLight* dLight = DirectionalLight::GetInstance();
	ImGui::DragFloat("X##Rot", &dLight->myDirection.x, 0.1f);
	ImGui::SameLine();
	ImGui::DragFloat("Y##Rot", &dLight->myDirection.y, 0.1f);
	ImGui::SameLine();
	ImGui::DragFloat("Z##Rot", &dLight->myDirection.z, 0.1f);

	ImGui::Text("Ambient Color");
	float ambientColor[4] = { dLight->myAmbientColor.x, dLight->myAmbientColor.y , dLight->myAmbientColor.z,dLight->myAmbientColor.w };
	ImGui::ColorPicker4("Ambient Color", ambientColor);
	dLight->myAmbientColor.x = ambientColor[0];
	dLight->myAmbientColor.y = ambientColor[1];
	dLight->myAmbientColor.z = ambientColor[2];
	dLight->myAmbientColor.w = ambientColor[3];


	ImGui::SetWindowPos(ImVec2(ImGui::GetWindowSize().x / 2, 0));

	ImGui::End();






	for (auto& model : myModels)
	{

		ImGui::GetWindowSize();
		ImGui::SetNextWindowSize(ImVec2(0, 0));
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

		ImGui::Begin(model.GetName().c_str(), nullptr, windowFlags);
		previousImGuiWindowHeight = ImGui::GetWindowSize().y;
		ImGui::PopStyleVar(2);
		ImGui::PushItemWidth(itemWidth);
		ImGui::Text("Position");
		ImGui::DragFloat("X##Pos", &model.myPosition.x, 0.1f);
		ImGui::SameLine();
		ImGui::DragFloat("Y##Pos", &model.myPosition.y, 0.1f);
		ImGui::SameLine();
		ImGui::DragFloat("Z##Pos", &model.myPosition.z, 0.1f);

		ImGui::Text("Rotation");

		ImGui::DragFloat("X##Rot", &model.myRotationAngles.x, 0.1f);
		ImGui::SameLine();
		ImGui::DragFloat("Y##Rot", &model.myRotationAngles.y, 0.1f);
		ImGui::SameLine();
		ImGui::DragFloat("Z##Rot", &model.myRotationAngles.z, 0.1f);

		ImGui::Text("Scale");
		ImGui::DragFloat("X##Scale", &model.myScale.x, 0.1f);
		ImGui::SameLine();
		ImGui::DragFloat("Y##Scale", &model.myScale.y, 0.1f);
		ImGui::SameLine();
		ImGui::DragFloat("Z##Scale", &model.myScale.z, 0.1f);
		ImGui::PopItemWidth();
		ImGui::SetWindowPos(ImVec2(myWidth - ImGui::GetWindowSize().x, (modelIndex * previousImGuiWindowHeight)));
		if (ImGui::Button("Delete model"))
		{
			myModels.erase(myModels.begin() + modelIndex);
			modelIndex--;
			ImGui::End();
			break;
		}

		ImGui::End();
		++modelIndex;
	}



	ImGui::StyleColorsLight();



	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(0, 0));
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);


	ImGui::PopStyleVar(2);

	if (gShowFBXWindow)
	{
		ShowFBXWindow(windowFlags);
	}
	else if (gShowTextureWindow)
	{
		ShowTextureWindow(windowFlags);
	}

	ImGui::End();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

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

	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	hr = mySwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));

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

	return true;
}

bool Graphics::InitScene()
{

	myCamera.SetPosition(0.0f, 10.0f, -10.0f);
	myCamera.SetProjectionValues(70.f, static_cast<float>(myWidth) / static_cast<float>(myHeight), 0.1f, 1000.f);
	LoadFBX("../bin/assets/meshes/other/man.fbx", L"../bin/assets/textures/man.jpg", L"../bin/PBRVertexShader.cso", L"../bin/PBRPixelShader.cso");
	
	//HRESULT hr = myPBRPixelShader.Init(myDevice.Get(), myDeviceContext.Get());
	//if (FAILED(hr))
	//{
	//	ErrorLog::Log(hr, "failed creating constant buffer.");
	//	return false;
	//}

	//DirectionalLight* dLight = DirectionalLight::GetInstance();
	//dLight->myDirectionalLightBuffer = myPBRPixelShader.Get();

	
	return true;
}

Model& Graphics::LoadFBX(std::string filePath, std::wstring aTexturePath, std::wstring aVertexShaderPath, std::wstring aPixelShaderPath)
{
	Model model;
	if (!model.Init(myDevice, myDeviceContext, filePath, aTexturePath, myCamera, aVertexShaderPath, aPixelShaderPath)) 
	{
		ErrorLog::Log("Failed initializing model");
	}
	myModels.push_back(model);
	return model;
}

void Graphics::LoadFBX(Model& aModel, std::string& filePath, std::wstring& aTexturePath, std::wstring& aVertexShaderPath, std::wstring& aPixelShaderPath)
{
	aModel;
	if (!aModel.Init(myDevice, myDeviceContext, filePath, aTexturePath, myCamera, aVertexShaderPath, aPixelShaderPath))
	{
		ErrorLog::Log("Failed initializing model");
	}
	myModels.push_back(aModel);
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
				LoadFBX(gFBXFilePath, gTextureFilePath, standardVertexShader, standardPixelShader);
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

		gridVertices.push_back(Vertex(-gridWidth * 0.5f, 0.0f, zPos, 0.0f, zPos / gridHeight,0,0,0));

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

	if (!myLineVertexShader.Init(myDevice, L"../bin/LineVertexShader.cso", layout, numElements))
	{
		return false;
	}

	if (!myLinePixelShader.Init(myDevice, L"../bin/LinePixelShader.cso"))
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

	myDeviceContext->UpdateSubresource(myGridConstantBuffer.Get(), 0, nullptr, &myGridConstantBuffer.myData, 0, 0);
	myGridConstantBuffer.ApplyChanges();
	myDeviceContext->VSSetConstantBuffers(1, 1, myGridConstantBuffer.GetAddressOf());

	myDeviceContext->IASetInputLayout(myLineVertexShader.GetInputLayout());
	myDeviceContext->VSSetShader(myLineVertexShader.GetShader(), NULL, 0);
	myDeviceContext->PSSetShader(myLinePixelShader.GetShader(), NULL, 0);

	myDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	myDeviceContext->IASetVertexBuffers(0, 1, myGridVertexBuffer.GetAddressOf(), &stride, &offset);
	myDeviceContext->IASetIndexBuffer(myGridIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	myDeviceContext->DrawIndexed(myGridIndexBuffer.GetBufferSize(), 0, 0);
}


