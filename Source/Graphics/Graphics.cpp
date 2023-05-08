#include "Graphics.h"
#include <fbxsdk.h>
#include <fbxsdk/core/math/fbxmath.h>
#include <fbxsdk/core/math/fbxquaternion.h>
#define VSYNC_ENABLED true

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

void Graphics::Render(const int& aFPS)
{

	float backgroundColor[] = { 0.0f, 0.0f, 0.0f,1.0f };
	mDeviceContext->ClearRenderTargetView(mRenderTargetView.Get(), backgroundColor);
	mDeviceContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	mDeviceContext->IASetInputLayout(mVertexShader.GetInputLayout());
	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mDeviceContext->RSSetState(mRasterizerState.Get());
	mDeviceContext->OMSetDepthStencilState(mDepthStencilState.Get(), 0);
	mDeviceContext->PSSetSamplers(0, 1, mSamplerState.GetAddressOf());

	mDeviceContext->VSSetShader(mVertexShader.GetShader(), NULL, 0);
	mDeviceContext->PSSetShader(mPixelShader.GetShader(), NULL, 0);

	UINT offset = 0;

	static float translationOffset[3] = { 0,0,0 };
	XMMATRIX world = XMMatrixTranslation(translationOffset[0], translationOffset[1], translationOffset[2]);

	mCBVSVertexShader.mData.mat = world * mCamera.GetViewMatrix() * mCamera.GetProjectionMatrix();
	mCBVSVertexShader.mData.mat = XMMatrixTranspose(mCBVSVertexShader.mData.mat);
	if (!mCBVSVertexShader.ApplyChanges())
	{
		return;
	}
	mDeviceContext->VSSetConstantBuffers(0, 1, mCBVSVertexShader.GetAddressOf());

	mCBPSPixelShader.mData.alpha = 1.0f;
	mCBPSPixelShader.ApplyChanges();
	mDeviceContext->PSSetConstantBuffers(0, 1, mCBPSPixelShader.GetAddressOf());



	mDeviceContext->PSSetShaderResources(0, 1, mTexture.GetAddressOf());
	mDeviceContext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), mVertexBuffer.GetStridePtr(), &offset);
	mDeviceContext->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	mDeviceContext->DrawIndexed(mIndexBuffer.GetBufferSize(), 0, 0);

	mSpriteBatch->Begin();
	std::wstring fpsCounter = L"FPS: ";
	fpsCounter += std::to_wstring(aFPS);
	mSpriteFont->DrawString(mSpriteBatch.get(), fpsCounter.c_str(), XMFLOAT2(0, 0), Colors::White, 0.0f, XMFLOAT2(0, 0), XMFLOAT2(1.0f, 1.0f));
	mSpriteBatch->End();

	static int counter = 0;
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("Test");
	//ImGui::Text("This is text");
	//if (ImGui::Button("CLICK"))
	//{
	//	++counter;
	//}
	//std::string clickCount = "Click Count: " + std::to_string(counter);
	//ImGui::Text(clickCount.c_str());

	ImGui::SameLine();
	ImGui::NewLine();
	ImGui::DragFloat3("Translation X/Y/Z", translationOffset, 0.1f, -5.f, 5.f);
	ImGui::End();
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
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION",0, DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,0}
	};

	UINT numElements = ARRAYSIZE(layout);

	if (!mVertexShader.Init(mDevice, L"../x64/Output/vertexshader.cso", layout, numElements))
	{
		return false;
	}

	if (!mPixelShader.Init(mDevice, L"../x64/Output/pixelshader.cso"))
	{
		return false;
	}

	return true;
}

bool Graphics::InitScene()
{



	FbxManager* fbxManager = FbxManager::Create();
	FbxIOSettings* ioSettings = FbxIOSettings::Create(fbxManager, IOSROOT);
	fbxManager->SetIOSettings(ioSettings);

	FbxImporter* fbxImporter = FbxImporter::Create(fbxManager, "");
	FbxScene* fbxScene = FbxScene::Create(fbxManager, "Scene");

	const char* filePath = "../Assets/Meshes/Scooter.fbx";
	bool success = fbxImporter->Initialize(filePath, -1, fbxManager->GetIOSettings());
	if (!success)
	{
		ErrorLog::Log("Failed loading fbx file");
		return false;
	}

	success = fbxImporter->Import(fbxScene);
	if (!success)
	{
		ErrorLog::Log("Failed importing fbx scene");
		return false;
	}

	fbxImporter->Destroy();




	FbxNode* rootNode = fbxScene->GetRootNode();
	if (rootNode)
	{
		std::vector<Vertex> vertices;
		std::vector<DWORD> indices;
		int numChildren = rootNode->GetChildCount();
		for (int i = 0; i < numChildren; i++)
		{
			FbxNode* childNode = rootNode->GetChild(i);
			FbxMesh* mesh = childNode->GetMesh();

			if (mesh)
			{
				// Triangulate the mesh
				if (!mesh->IsTriangleMesh())
				{
					FbxGeometryConverter converter(mesh->GetFbxManager());
					converter.Triangulate(mesh, true);
				}

				int numVertices = mesh->GetControlPointsCount();
				std::vector<FbxVector4> controlPoints(numVertices);

				// Retrieve UV coordinates
				FbxLayerElementUV* uvLayer = mesh->GetLayer(0)->GetUVs();
				FbxArray<FbxVector2> uvCoords;
				mesh->GetPolygonVertexUVs(uvLayer->GetName(), uvCoords);

				// Extract the control points and UV coordinates
				for (int j = 0; j < numVertices; j++)
				{
					FbxVector4 vertex = mesh->GetControlPointAt(j);
					float x = static_cast<float>(vertex[0]);
					float y = static_cast<float>(vertex[1]);
					float z = static_cast<float>(vertex[2]);

					// Retrieve the UV coordinate for the vertex
					FbxVector2 uv = uvCoords[j];

					Vertex v(x, y, z, static_cast<float>(uv[0]), static_cast<float>(uv[1]));
					vertices.push_back(v);
					controlPoints[j] = vertex;
				}

				// Extract the triangles
				int numPolygons = mesh->GetPolygonCount();
				for (int polygonIndex = 0; polygonIndex < numPolygons; polygonIndex++)
				{
					int polygonSize = mesh->GetPolygonSize(polygonIndex);
					if (polygonSize != 3)
					{
						// Skip polygons that are not triangles
						continue;
					}

					for (int vertexIndex = 0; vertexIndex < polygonSize; vertexIndex++)
					{
						// Get the control point index for the vertex
						int controlPointIndex = mesh->GetPolygonVertex(polygonIndex, vertexIndex);

						// Add the index to the indices vector
						indices.push_back(static_cast<DWORD>(controlPointIndex));
					}
				}
			}
		}

		HRESULT hr = mVertexBuffer.Init(mDevice.Get(), &vertices[0], vertices.size());
		if (FAILED(hr))
		{
			ErrorLog::Log(hr, "Failed to create vertex buffer.");
			return false;
		}

		hr = mIndexBuffer.Init(mDevice.Get(), &indices[0], indices.size());
		if (FAILED(hr))
		{
			ErrorLog::Log(hr, "Failed to create index buffer.");
			return false;
		}
	}

	HRESULT hr = CreateWICTextureFromFile(mDevice.Get(), L"../Assets/Textures/Scooter.png", nullptr, mTexture.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "fucked up creating texture from file.");
		return false;
	}

	hr = mCBVSVertexShader.Init(mDevice.Get(), mDeviceContext.Get());
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "fucked up creating constant buffer.");
		return false;
	}
	hr = mCBPSPixelShader.Init(mDevice.Get(), mDeviceContext.Get());
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "fucked up creating constant buffer.");
		return false;
	}

	fbxScene->Destroy();
	ioSettings->Destroy();
	fbxManager->Destroy();

	mCamera.SetPosition(0.0f, 0.0f, -2.0f);
	mCamera.SetProjectionValues(90.f, static_cast<float>(mWidth) / static_cast<float>(mHeight), 0.1f, 1000.f);
	return true;
}