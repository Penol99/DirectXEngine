#include "Shaders.h"

VertexShader::VertexShader()
	:myShader(nullptr),
	myShaderBuffer(nullptr)
{
}

bool VertexShader::Init(ComPtr<ID3D11Device>& aDevice, std::wstring aShaderPath, D3D11_INPUT_ELEMENT_DESC* aLayoutDesc, UINT aNumElements)
{
	HRESULT hr = D3DReadFileToBlob(aShaderPath.c_str(), myShaderBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		std::wstring errorMsg = L"failed loading shader: ";
		errorMsg += aShaderPath;
		ErrorLog::Log(hr, errorMsg);
		return false;

	}
	hr = aDevice->CreateInputLayout(aLayoutDesc, aNumElements, GetBuffer()->GetBufferPointer(), GetBuffer()->GetBufferSize(), myInputLayout.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "failed creating input layout, rip.");
		return false;
	}

	hr = aDevice->CreateVertexShader(myShaderBuffer.Get()->GetBufferPointer(), myShaderBuffer.Get()->GetBufferSize(),NULL,myShader.GetAddressOf());
	if (FAILED(hr))
	{
		std::wstring errorMsg = L"failed creating vertex shader: ";
		errorMsg += aShaderPath;
		ErrorLog::Log(hr, errorMsg);
		return false;
	}
	return true;
}

ID3D11VertexShader* VertexShader::GetShader()
{
	return myShader.Get();
}

ID3D10Blob* VertexShader::GetBuffer()
{
	return myShaderBuffer.Get();
}

ID3D11InputLayout* VertexShader::GetInputLayout()
{
	return myInputLayout.Get();
}


bool PixelShader::Init(ComPtr<ID3D11Device>& aDevice, std::wstring aShaderPath)
{
	HRESULT hr = D3DReadFileToBlob(aShaderPath.c_str(), myShaderBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		std::wstring errorMsg = L"failed loading shader: ";
		errorMsg += aShaderPath;
		ErrorLog::Log(hr, errorMsg);
		return false;

	}

	hr = aDevice->CreatePixelShader(myShaderBuffer.Get()->GetBufferPointer(), myShaderBuffer.Get()->GetBufferSize(), NULL, myShader.GetAddressOf());
	if (FAILED(hr))
	{
		std::wstring errorMsg = L"failed creating pixel shader: ";
		errorMsg += aShaderPath;
		ErrorLog::Log(hr, errorMsg);
		return false;
	}
	return true;
}

ID3D11PixelShader* PixelShader::GetShader()
{
	return myShader.Get();
}

ID3D10Blob* PixelShader::GetBuffer()
{
	return myShaderBuffer.Get();
}
