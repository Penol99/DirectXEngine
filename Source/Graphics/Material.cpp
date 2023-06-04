//#include "Material.h"
//#include <WICTextureLoader.h>
//bool Material::Init(ComPtr<ID3D11Device>& aDevice, const std::wstring& aVertexShaderPath, const std::wstring& aPixelShaderPath)
//{
//    if (!myVertexShader.Init(aDevice, aVertexShaderPath))
//        return false;
//
//    if (!myPixelShader.Init(aDevice, aPixelShaderPath))
//        return false;
//
//
//    myDevice = aDevice.Get();
//    return true;
//}
//
//void Material::Render(ID3D11DeviceContext* aDeviceContext)
//{
//    aDeviceContext->IASetInputLayout(myVertexShader.GetInputLayout());
//    aDeviceContext->VSSetShader(myVertexShader.GetShader(), NULL, 0);
//    aDeviceContext->PSSetShader(myPixelShader.GetShader(), NULL, 0);
//
//
//    aDeviceContext->PSSetShaderResources(0, 1, myTexture.GetAddressOf());
//    aDeviceContext->PSSetShaderResources(3, 1, myReflectionTexture.GetAddressOf());
//}
//
//void Material::SetColor(const DirectX::XMFLOAT4& color)
//{
//    myColor = color;
//}
//
//void Material::SetTexture(std::wstring aTexturePath)
//{
//    HRESULT hr = DirectX::CreateWICTextureFromFile(myDevice, aTexturePath.c_str(), nullptr, myTexture.GetAddressOf());
//    if (FAILED(hr))
//    {
//        ErrorLog::Log(hr, "failed creating texture from file.");
//
//    }
//}
//
//void Material::Bind(ID3D11DeviceContext* aDeviceContext)
//{
//    aDeviceContext->IASetInputLayout(myVertexShader.GetInputLayout());
//    aDeviceContext->VSSetShader(myVertexShader.GetShader(), nullptr, 0);
//    aDeviceContext->PSSetShader(myPixelShader.GetShader(), nullptr, 0);
//    aDeviceContext->PSSetShaderResources(0, 1, &myTexture);
//}