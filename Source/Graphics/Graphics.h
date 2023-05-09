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
//#include <assimp/Importer.hpp>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h>
#include "Model.h"



using namespace Microsoft::WRL;
class Graphics
{
public:
	bool Init(HWND hwnd, int aWidth, int aHeight);
	void Render(const int& aFPS);
	Camera mCamera;
private:
	bool InitDirectX(HWND hwnd);
	bool InitShaders();
	bool InitScene();
	//void ProcessNode(const aiNode* node, const aiScene* scene, std::vector<Vertex>& vertices, std::vector<DWORD>& indices);
	ComPtr<ID3D11Device> mDevice;
	ComPtr<ID3D11DeviceContext> mDeviceContext;
	ComPtr<IDXGISwapChain> mSwapChain;
	ComPtr<ID3D11RenderTargetView> mRenderTargetView;


	VertexShader mVertexShader; 
	PixelShader mPixelShader;

	ConstantBuffer<CB_VS_VertexShader> mCBVSVertexShader;
	ConstantBuffer<CB_PS_PixelShader> mCBPSPixelShader;
	VertexBuffer<Vertex> mVertexBuffer;
	IndexBuffer mIndexBuffer;

	ComPtr<ID3D11DepthStencilView> mDepthStencilView;
	ComPtr<ID3D11Texture2D> mDepthStencilBuffer;
	ComPtr<ID3D11DepthStencilState> mDepthStencilState;

	ComPtr<ID3D11RasterizerState> mRasterizerState;
	ComPtr<ID3D11BlendState> mBlendState;

	std::unique_ptr<DirectX::SpriteBatch> mSpriteBatch;
	std::unique_ptr<DirectX::SpriteFont> mSpriteFont;

	ComPtr<ID3D11SamplerState> mSamplerState;
	ComPtr<ID3D11ShaderResourceView> mTexture;
	Model myPlayer;
	Model myScooter;


	int mWidth;
	int mHeight;

};
