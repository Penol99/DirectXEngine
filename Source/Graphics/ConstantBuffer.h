#pragma once
#ifndef ConstantBuffer_h__
#define ConstantBuffer_h__
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
	ComPtr<ID3D11Buffer> mBuffer;
	ID3D11DeviceContext* mDeviceContext = nullptr;
public:
	ConstantBuffer() {}

	T mData;

	ID3D11Buffer* Get()const
	{
		return mBuffer.Get();
	}
	ID3D11Buffer* const* GetAddressOf()const
	{
		return mBuffer.GetAddressOf();
	}


	HRESULT Init(ID3D11Device* aDevice, ID3D11DeviceContext* aDeviceContext)
	{
		if (mBuffer.Get() != nullptr)
		{
			mBuffer.Reset();
		}


		mDeviceContext = aDeviceContext;
		D3D11_BUFFER_DESC constantBufferDesc;
		constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		constantBufferDesc.MiscFlags = 0;
		constantBufferDesc.ByteWidth = static_cast<UINT>(sizeof(CB_VS_VertexShader)) + (16 - (sizeof(CB_VS_VertexShader)));
		constantBufferDesc.StructureByteStride = 0;

		return aDevice->CreateBuffer(&constantBufferDesc, 0, mBuffer.GetAddressOf());

	}

	bool ApplyChanges() 
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		HRESULT hr = mDeviceContext->Map(mBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(hr))
		{
			ErrorLog::Log(hr, "fucked up mapping constant buffer");
			return false;
		}
		CopyMemory(mappedResource.pData, &mData, sizeof(CB_VS_VertexShader));
		mDeviceContext->Unmap(mBuffer.Get(), 0);
		return true;
	}

};
#endif // ConstantBuffer_h__