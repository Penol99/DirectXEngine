#pragma once
#include "../ErrorLog.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include "d3dcommon.h"
#include <wrl/client.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dCompiler.lib")
using namespace Microsoft::WRL;
class VertexShader
{
public:
	VertexShader();
	bool Init(ComPtr<ID3D11Device>& aDevice, std::wstring aShaderPath, D3D11_INPUT_ELEMENT_DESC* aDesc, UINT numElements);
	ID3D11VertexShader* GetShader();
	ID3D10Blob* GetBuffer();
	ID3D11InputLayout* GetInputLayout();
private:
	ComPtr<ID3D11VertexShader> mShader;
	ComPtr<ID3D10Blob> mShaderBuffer;
	ComPtr<ID3D11InputLayout> mInputLayout;

};
class PixelShader
{
public:
	bool Init(ComPtr<ID3D11Device>& aDevice, std::wstring aShaderPath);
	ID3D11PixelShader* GetShader();
	ID3D10Blob* GetBuffer();
private:
	ComPtr<ID3D11PixelShader> mShader;
	ComPtr<ID3D10Blob> mShaderBuffer;
};