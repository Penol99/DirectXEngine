#include "Mesh.h"
#include "../ErrorLog.h"
#include <d3d11.h> 



Mesh::Mesh()
{
	mVertexBuffer;
	mIndexBuffer;
}

void Mesh::Render(ID3D11DeviceContext* aDeviceContext)
{

	UINT offset = 0;
	aDeviceContext->IASetInputLayout(mVertexShader.GetInputLayout());
	aDeviceContext->VSSetShader(mVertexShader.GetShader(), NULL, 0);
	aDeviceContext->PSSetShader(mPixelShader.GetShader(), NULL, 0);


	aDeviceContext->PSSetShaderResources(0, 1, mTexture.GetAddressOf());
	aDeviceContext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), mVertexBuffer.GetStridePtr(), &offset);
	aDeviceContext->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	aDeviceContext->DrawIndexed(mIndexBuffer.GetBufferSize(), 0, 0);
}

bool Mesh::Init(ComPtr<ID3D11Device>& aDevice, std::vector<Vertex>& vertices, std::vector<DWORD>& indices, std::wstring& aTexturePath)
{
	HRESULT hr = mVertexBuffer.Init(aDevice.Get(), &vertices[0], vertices.size());
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "Failed to create vertex buffer.");
		return false;

	}

	hr = mIndexBuffer.Init(aDevice.Get(), &indices[0], indices.size());
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "Failed to create index buffer.");
		return false;

	}
	hr = DirectX::CreateWICTextureFromFile(aDevice.Get(), aTexturePath.c_str(), nullptr, mTexture.GetAddressOf());
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

	if (!mVertexShader.Init(aDevice, L"../x64/Output/vertexshader.cso", layout, numElements))
	{
		return false;
	}

	if (!mPixelShader.Init(aDevice, L"../x64/Output/pixelshader.cso"))
	{
		return false;
	}

	return true;
}
