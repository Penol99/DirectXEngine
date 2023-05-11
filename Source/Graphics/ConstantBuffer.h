#pragma once

#include <d3d11.h>
#include "ConstantBufferTypes.h"
#include <wrl/client.h>
#include "../ErrorLog.h"
using namespace Microsoft::WRL;

template<class T>
class ConstantBuffer
{
private:
	ConstantBuffer(const ConstantBuffer<T>& rhs);
	ComPtr<ID3D11Buffer> myBuffer;
	ID3D11DeviceContext* myDeviceContext = nullptr;
public:
	ConstantBuffer() {}

	T myData;

	ID3D11Buffer* Get()const
	{
		return myBuffer.Get();
	}
	ID3D11Buffer* const* GetAddressOf()const
	{
		return myBuffer.GetAddressOf();
	}


	HRESULT Init(ID3D11Device* aDevice, ID3D11DeviceContext* aDeviceContext)
	{
		if (myBuffer.Get() != nullptr)
		{
			myBuffer.Reset();
		}


		myDeviceContext = aDeviceContext;
		D3D11_BUFFER_DESC constantBufferDesc;
		constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		constantBufferDesc.MiscFlags = 0;
		constantBufferDesc.ByteWidth = static_cast<UINT>(sizeof(CB_VS_VertexShader)) + (16 - (sizeof(CB_VS_VertexShader)));
		constantBufferDesc.StructureByteStride = 0;

		return aDevice->CreateBuffer(&constantBufferDesc, 0, myBuffer.GetAddressOf());

	}

	bool ApplyChanges() 
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		HRESULT hr = myDeviceContext->Map(myBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(hr))
		{
			ErrorLog::Log(hr, "failed mapping constant buffer");
			return false;
		}
		CopyMemory(mappedResource.pData, &myData, sizeof(CB_VS_VertexShader));
		myDeviceContext->Unmap(myBuffer.Get(), 0);
		return true;
	}

};
