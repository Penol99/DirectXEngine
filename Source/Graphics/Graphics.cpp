#include "Graphics.h"

//#include <fbxsdk.h>
//#include <fbxsdk/core/math/fbxmath.h>
//#include <fbxsdk/core/math/fbxquaternion.h>


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

	//mDeviceContext->IASetInputLayout(mVertexShader.GetInputLayout());
	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mDeviceContext->RSSetState(mRasterizerState.Get());
	mDeviceContext->OMSetDepthStencilState(mDepthStencilState.Get(), 0);
	mDeviceContext->PSSetSamplers(0, 1, mSamplerState.GetAddressOf());

	//mDeviceContext->VSSetShader(mVertexShader.GetShader(), NULL, 0);
	//mDeviceContext->PSSetShader(mPixelShader.GetShader(), NULL, 0);

	//UINT offset = 0;

	static float worldScale[3] = { .5f,.5f,.5f };
	static float worldTranslationOffset[3] = { 0,0,0 };
	// Scale the object uniformly by 0.5
	XMMATRIX scale = XMMatrixScaling(worldScale[0], worldScale[1], worldScale[2]);
	XMMATRIX translationOffset = XMMatrixTranslation(worldTranslationOffset[0], worldTranslationOffset[1], worldTranslationOffset[2]);
	XMMATRIX world = scale * translationOffset;

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


	myPlayer.Render(mDeviceContext.Get());
	myScooter.Render(mDeviceContext.Get());
	//mDeviceContext->PSSetShaderResources(0, 1, mTexture.GetAddressOf());
	//mDeviceContext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), mVertexBuffer.GetStridePtr(), &offset);
	//mDeviceContext->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	//mDeviceContext->DrawIndexed(mIndexBuffer.GetBufferSize(), 0, 0);

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


	ImGui::SameLine();
	ImGui::NewLine();
	ImGui::SetWindowPos(ImVec2( 0, 0 ));
	ImGui::SetWindowSize(ImVec2(mWidth, 150));
	ImGui::DragFloat3("World Scale", worldScale, 0.1f, -5.f, 5.f);
	ImGui::DragFloat3("World Translation", worldTranslationOffset, 0.1f, -5.f, 5.f);
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

	return true;
}

bool Graphics::InitScene()
{

	myPlayer.Init(mDevice, "../Assets/Meshes/SK_Player.fbx", L"../Assets/Textures/SK_Player_c.dds");
	myScooter.Init(mDevice, "../Assets/Meshes/Scooter.fbx", L"../Assets/Textures/Scooter.png");

	HRESULT hr = mCBVSVertexShader.Init(mDevice.Get(), mDeviceContext.Get());
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

	mCamera.SetPosition(0.0f, 0.0f, -2.0f);
	mCamera.SetProjectionValues(90.f, static_cast<float>(mWidth) / static_cast<float>(mHeight), 0.1f, 1000.f);
	return true;
}

//void Graphics::ProcessNode(const aiNode* node, const aiScene* scene, std::vector<Vertex>& vertices, std::vector<DWORD>& indices)
//{
//	// Process mesh data
//	for (unsigned int i = 0; i < node->mNumMeshes; i++)
//	{
//		const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
//
//		// Process vertices
//		for (unsigned int j = 0; j < mesh->mNumVertices; j++)
//		{
//			const aiVector3D& vertex = mesh->mVertices[j];
//			const aiVector3D& uv = mesh->mTextureCoords[0][j];
//
//			Vertex v(vertex.x, vertex.y, vertex.z, uv.x, uv.y);
//			vertices.push_back(v);
//		}
//
//		// Process indices
//		for (unsigned int j = 0; j < mesh->mNumFaces; j++)
//		{
//			const aiFace& face = mesh->mFaces[j];
//			for (unsigned int k = 0; k < face.mNumIndices; k++)
//			{
//				indices.push_back(face.mIndices[k]);
//			}
//		}
//	}
//
//	// Process child nodes recursively
//	for (unsigned int i = 0; i < node->mNumChildren; i++)
//	{
//		ProcessNode(node->mChildren[i], scene, vertices, indices);
//	}
//}