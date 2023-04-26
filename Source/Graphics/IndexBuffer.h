#pragma once
#ifndef IndexBuffer_h__
#define IndexBuffer_h__
#include <d3d11.h>
#include <wrl/client.h>
using namespace Microsoft::WRL;

class IndexBuffer
{
private:
	IndexBuffer(const IndexBuffer& rhs);
	ComPtr<ID3D11Buffer> mBuffer;
	UINT mBufferSize = 0;
public:
	IndexBuffer() {}

	ID3D11Buffer* Get()const
	{
		return mBuffer.Get();
	}
	ID3D11Buffer* const* GetAddressOf()const
	{
		return mBuffer.GetAddressOf();
	}

	UINT GetBufferSize()const
	{
		return mBufferSize;
	}

	HRESULT Init(ID3D11Device* aDevice, DWORD* someData, UINT numIndicies)
	{
		if (mBuffer.Get() != nullptr)
		{
			mBuffer.Reset();
		}
		mBufferSize = numIndicies;

		D3D11_BUFFER_DESC indexBufferDesc;
		ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(DWORD) * numIndicies;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA indexBufferData;
		ZeroMemory(&indexBufferData, sizeof(indexBufferData));

		indexBufferData.pSysMem = someData;

		return aDevice->CreateBuffer(&indexBufferDesc, &indexBufferData, mBuffer.GetAddressOf());

	}

};
#endif // IndexBuffer_h__