#pragma once
#include "AdapterReader.h"
#include "../Shaders/Shaders.h"
#include "Vertex.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <WICTextureLoader.h>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "Camera.h"
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_win32.h"
#include "../ImGui/imgui_impl_dx11.h"
#include "../Entity Component System/GameObject.h"
#include <filesystem>

//#include "TerrainGenerator.h"
#include "Model.h"
#include "../Timer.h"



using namespace Microsoft::WRL;
class Graphics
{

public:
	bool Init(HWND hwnd, int aWidth, int aHeight, Timer& aTimer);
	void Render(const int& aFPS, const float& aDeltaTime);
	void RenderImGui();
	void Resize(int aWidth, int aHeight);
	Camera myCamera;
private:
	bool InitDirectX(HWND hwnd);
	bool InitScene();
	bool CreateSwapChain(HWND hwnd);
	bool InitGrid();
	void RenderGrid();
	void RenderFileHierarchy(const std::filesystem::path& aDirectory);
	ComPtr<ID3D11Device> myDevice;
	ComPtr<ID3D11DeviceContext> myDeviceContext;
	ComPtr<IDXGISwapChain> mySwapChain;
	ComPtr<ID3D11RenderTargetView> myRenderTargetView;


	ConstantBuffer<CB_VS_StandardShader> myCBVSVertexShader;

	ComPtr<ID3D11DepthStencilView> myDepthStencilView;
	ComPtr<ID3D11Texture2D> myDepthStencilBuffer;
	ComPtr<ID3D11DepthStencilState> myDepthStencilState;

	ComPtr<ID3D11RasterizerState> myRasterizerState;
	ComPtr<ID3D11BlendState> myBlendState;

	std::unique_ptr<DirectX::SpriteBatch> mySpriteBatch;
	std::unique_ptr<DirectX::SpriteFont> mySpriteFont;

	ComPtr<ID3D11SamplerState> mySamplerState;
	ComPtr<ID3D11ShaderResourceView> myTexture;

	std::vector<GameObject*> myGameObjects;

	VertexBuffer<Vertex> myGridVertexBuffer;
	IndexBuffer myGridIndexBuffer;
	ConstantBuffer<CB_VS_StandardShader> myGridConstantBuffer;

	VertexShader myLineVertexShader;
	PixelShader myLinePixelShader;
	ConstantBuffer<CB_PS_PBRShader> myPBRPixelShader;

	ComPtr<ID3D11ShaderResourceView> myCameraShaderResourceView;
	ComPtr<ID3D11RenderTargetView> myCameraRenderTargetView;
	ComPtr<ID3D11Texture2D> myCameraTexture;
	//TerrainGenerator myTerrainGenerator;

	Timer* myTimer;
	int myWidth;
	int myHeight;

	float myCameraSpeed = 10.0f;
	HWND myHandle;
	friend class Engine;
};
