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
//#include "TerrainGenerator.h"
#include "Model.h"



using namespace Microsoft::WRL;
class Graphics
{

public:
	bool Init(HWND hwnd, int aWidth, int aHeight, Timer& aTimer);
	void Render(const int& aFPS, const float& aDeltaTime);
	void RenderImGui();
	Camera myCamera;
private:
	bool InitDirectX(HWND hwnd);
	bool InitScene();
	Model& LoadFBX(std::string filePath, std::wstring aTexturePath, std::wstring aVertexShaderPath, std::wstring aPixelShaderPath);
	void LoadFBX(Model& aModel, std::string& filePath, std::wstring& aTexturePath, std::wstring& aVertexShaderPath, std::wstring& aPixelShaderPath);
	void ShowFBXWindow(ImGuiWindowFlags& someFlags);
	void ShowTextureWindow(ImGuiWindowFlags& someFlags);
	bool InitGrid();
	void RenderGrid();
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

	std::vector<Model> myModels;

	VertexBuffer<Vertex> myGridVertexBuffer;
	IndexBuffer myGridIndexBuffer;
	ConstantBuffer<CB_VS_StandardShader> myGridConstantBuffer;

	VertexShader myLineVertexShader;
	PixelShader myLinePixelShader;
	ConstantBuffer<CB_PS_PBRShader> myPBRPixelShader;
	//TerrainGenerator myTerrainGenerator;

	Timer* myTimer;
	int myWidth;
	int myHeight;

	float myCameraSpeed = 10.0f;

	friend class Engine;
};
