#include "Mesh.h"
#include "../ErrorLog.h"
#include <d3d11.h> 



Mesh::Mesh()
{
	myVertexBuffer;
	myIndexBuffer;
}

void Mesh::Render(ID3D11DeviceContext* aDeviceContext)
{

	UINT offset = 0;
	aDeviceContext->IASetInputLayout(myVertexShader.GetInputLayout());
	aDeviceContext->VSSetShader(myVertexShader.GetShader(), NULL, 0);
	aDeviceContext->PSSetShader(myPixelShader.GetShader(), NULL, 0);


	aDeviceContext->PSSetShaderResources(0, 1, myTexture.GetAddressOf());
	aDeviceContext->IASetVertexBuffers(0, 1, myVertexBuffer.GetAddressOf(), myVertexBuffer.GetStridePtr(), &offset);
	aDeviceContext->IASetIndexBuffer(myIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	aDeviceContext->DrawIndexed(myIndexBuffer.GetBufferSize(), 0, 0);
}

bool Mesh::Init(ComPtr<ID3D11Device>& aDevice, std::vector<Vertex>& vertices, std::vector<DWORD>& indices,UINT numVertices, UINT numIndices, std::wstring& aTexturePath, std::wstring& aVertexShaderPath,std::wstring& aPixelShaderPath)
{
	HRESULT hr = myVertexBuffer.Init(aDevice.Get(), &vertices[0], numVertices);
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "Failed to create vertex buffer.");
		return false;

	}

	hr = myIndexBuffer.Init(aDevice.Get(), &indices[0], numIndices);
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "Failed to create index buffer.");
		return false;

	}
	hr = DirectX::CreateWICTextureFromFile(aDevice.Get(), aTexturePath.c_str(), nullptr, myTexture.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "failed creating texture from file.");
		return false;

	}

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION",0, DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,0}
	};

	UINT numElements = ARRAYSIZE(layout);

	if (!myVertexShader.Init(aDevice, aVertexShaderPath, layout, numElements))
	{
		return false;
	}

	if (!myPixelShader.Init(aDevice, aPixelShaderPath))
	{
		return false;
	}

	return true;
}
