#pragma once
#include "Vertex.h"
#include <vector>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "../Shaders/Shaders.h"
#include <WICTextureLoader.h>
class Mesh
{
	friend class Model;
public:
	
	Mesh();
	Mesh(const Mesh& other)
	{
		// Copy other members
		// ...
		mVertexBuffer = other.mVertexBuffer;
		mIndexBuffer = other.mIndexBuffer;
		mVertexShader = other.mVertexShader;
		mPixelShader = other.mPixelShader;
		mTexture = other.mTexture;

		// Copy mStride
//		mStride = std::make_unique<UINT>(*other.mStride);
	}
	void Render(ID3D11DeviceContext* aDeviceContext);

private:
	bool Init(ComPtr<ID3D11Device>& aDevice, std::vector<Vertex>& vertices, std::vector<DWORD>& indices, std::wstring& aTexturePath);

	VertexBuffer<Vertex> mVertexBuffer; 
	IndexBuffer mIndexBuffer;

	VertexShader mVertexShader;
	PixelShader mPixelShader;
	ComPtr<ID3D11ShaderResourceView> mTexture;
	std::shared_ptr<UINT> mStride;
};

