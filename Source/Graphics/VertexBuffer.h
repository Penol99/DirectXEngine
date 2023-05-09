#pragma once
#ifndef VertexBuffer_h__
#define VertexBuffer_h__
#include <d3d11.h>
#include <wrl/client.h>
#include <memory>
using namespace Microsoft::WRL;

template<class T>
class VertexBuffer
{
private:
	VertexBuffer(const VertexBuffer<T>& rhs);
	ComPtr<ID3D11Buffer> mBuffer;
	std::shared_ptr<UINT> mStride;
	UINT mBufferSize = 0;
public:
	VertexBuffer(){}

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
	const UINT GetStride() const
	{
		return *mStride;
	}
	const UINT *GetStridePtr() const
	{
		return mStride.get();
	}
	HRESULT Init(ID3D11Device* aDevice, T* someData, UINT numVertices)
	{
		if (mBuffer.Get() != nullptr)
		{
			mBuffer.Reset();
		}
		mBufferSize = numVertices;
		if (mStride == nullptr)
		{
			mStride = std::make_shared<UINT>(sizeof(T));
		}

		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(T) * numVertices;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA vertexBufferData;
		ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));

		vertexBufferData.pSysMem = someData;

		return aDevice->CreateBuffer(&vertexBufferDesc, &vertexBufferData, mBuffer.GetAddressOf());
		
	}

};
#endif // VertexBuffer_h__