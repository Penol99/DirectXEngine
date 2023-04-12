#include "Shaders.h"

VertexShader::VertexShader()
	:mShader(nullptr),
	mShaderBuffer(nullptr)
{
}

bool VertexShader::Init(ComPtr<ID3D11Device>& aDevice, std::wstring aShaderPath, D3D11_INPUT_ELEMENT_DESC* aLayoutDesc, UINT aNumElements)
{
	HRESULT hr = D3DReadFileToBlob(aShaderPath.c_str(), mShaderBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		std::wstring errorMsg = L"fucked up loading shader: ";
		errorMsg += aShaderPath;
		ErrorLog::Log(hr, errorMsg);
		return false;

	}
	hr = aDevice->CreateInputLayout(aLayoutDesc, aNumElements, GetBuffer()->GetBufferPointer(), GetBuffer()->GetBufferSize(), mInputLayout.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "fucked up creating input layout, rip.");
		return false;
	}

	hr = aDevice->CreateVertexShader(mShaderBuffer.Get()->GetBufferPointer(), mShaderBuffer.Get()->GetBufferSize(),NULL,mShader.GetAddressOf());
	if (FAILED(hr))
	{
		std::wstring errorMsg = L"fucked up creating vertex shader: ";
		errorMsg += aShaderPath;
		ErrorLog::Log(hr, errorMsg);
		return false;
	}
	return true;
}

ID3D11VertexShader* VertexShader::GetShader()
{
	return mShader.Get();
}

ID3D10Blob* VertexShader::GetBuffer()
{
	return mShaderBuffer.Get();
}

ID3D11InputLayout* VertexShader::GetInputLayout()
{
	return mInputLayout.Get();
}


bool PixelShader::Init(ComPtr<ID3D11Device>& aDevice, std::wstring aShaderPath)
{
	HRESULT hr = D3DReadFileToBlob(aShaderPath.c_str(), mShaderBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		std::wstring errorMsg = L"fucked up loading shader: ";
		errorMsg += aShaderPath;
		ErrorLog::Log(hr, errorMsg);
		return false;

	}

	hr = aDevice->CreatePixelShader(mShaderBuffer.Get()->GetBufferPointer(), mShaderBuffer.Get()->GetBufferSize(), NULL, mShader.GetAddressOf());
	if (FAILED(hr))
	{
		std::wstring errorMsg = L"fucked up creating pixel shader: ";
		errorMsg += aShaderPath;
		ErrorLog::Log(hr, errorMsg);
		return false;
	}
	return true;
}

ID3D11PixelShader* PixelShader::GetShader()
{
	return mShader.Get();
}

ID3D10Blob* PixelShader::GetBuffer()
{
	return mShaderBuffer.Get();
}
