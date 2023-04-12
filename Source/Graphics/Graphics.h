#pragma once
#include "AdapterReader.h"
#include "../Shaders/Shaders.h"
#include "Vertex.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <WICTextureLoader.h>
using namespace Microsoft::WRL;
class Graphics
{
public:
	bool Init(HWND hwnd, int aWidth, int aHeight);
	void Render();
private:
	bool InitDirectX(HWND hwnd,int aWidth,int aHeight);
	bool InitShaders();
	bool InitScene();
	bool CreateVertexBuffer(ComPtr<ID3D11Buffer>& aVertexBuffer, Vertex* someVertices, int numElements, std::wstring aTexturePath);
	ComPtr<ID3D11Device> mDevice;
	ComPtr<ID3D11DeviceContext> mDeviceContext;
	ComPtr<IDXGISwapChain> mSwapChain;
	ComPtr<ID3D11RenderTargetView> mRenderTargetView;


	VertexShader mVertexShader; 
	PixelShader mPixelShader;

	ComPtr<ID3D11Buffer> mVertexBuffer;


	ComPtr<ID3D11DepthStencilView> mDepthStencilView;
	ComPtr<ID3D11Texture2D> mDepthStencilBuffer;
	ComPtr<ID3D11DepthStencilState> mDepthStencilState;

	ComPtr<ID3D11RasterizerState> mRasterizerState;

	std::unique_ptr<DirectX::SpriteBatch> mSpriteBatch;
	std::unique_ptr<DirectX::SpriteFont> mSpriteFont;

	ComPtr<ID3D11SamplerState> mSamplerState;
	ComPtr<ID3D11ShaderResourceView> mTexture;
};
