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
	D3D11_INPUT_ELEMENT_DESC layout[3];
	
	layout[0].SemanticName = "POSITION";
	layout[0].SemanticIndex = 0;
	layout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	layout[0].InputSlot = 0;
	layout[0].AlignedByteOffset = 0;
	layout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layout[0].InstanceDataStepRate = 0;

	layout[1].SemanticName = "NORMAL";
	layout[1].SemanticIndex = 0;
	layout[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	layout[1].InputSlot = 0;
	layout[1].AlignedByteOffset = 12;
	layout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layout[1].InstanceDataStepRate = 0;

	layout[2].SemanticName = "TEXCOORD";
	layout[2].SemanticIndex = 0;
	layout[2].Format = DXGI_FORMAT_R32G32_FLOAT;
	layout[2].InputSlot = 0;
	layout[2].AlignedByteOffset = 24;
	layout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layout[2].InstanceDataStepRate = 0;
	

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
