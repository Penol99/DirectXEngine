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
		myVertexBuffer = other.myVertexBuffer;
		myIndexBuffer = other.myIndexBuffer;
		myVertexShader = other.myVertexShader;
		myPixelShader = other.myPixelShader;
		myTexture = other.myTexture;
	}
	void Render(ID3D11DeviceContext* aDeviceContext);

private:
	bool Init(ComPtr<ID3D11Device>& aDevice, std::vector<Vertex>& vertices, std::vector<DWORD>& indices, UINT numVertices, UINT numIndices, std::wstring& aTexturePath, std::wstring& aVertexShaderPath, std::wstring& aPixelShaderPath);

	VertexBuffer<Vertex> myVertexBuffer; 
	IndexBuffer myIndexBuffer;

	VertexShader myVertexShader;
	PixelShader myPixelShader;
	ComPtr<ID3D11ShaderResourceView> myTexture;
	std::shared_ptr<UINT> myStride;
};

