#pragma once
#include "Component.h"
#include <string>
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>
#include "../../Shaders/Shaders.h"
#include "../../Graphics/ConstantBuffer.h"
#include "TransformComponent.h"
#include <memory>
#include <vector>
using namespace Microsoft::WRL;

class MaterialComponent : public Component
{
public:
    bool Init(const std::wstring& aVertexShaderPath, const std::wstring& aPixelShaderPath);
    void Render();
    void SetColor(const DirectX::XMFLOAT4& aColor);
    void SetTexture(std::wstring aTexturePath);
    void SetReflectionTexture(std::wstring aTexturePath);

private:
    VertexShader myVertexShader;
    PixelShader myPixelShader;
    DirectX::XMFLOAT4 myColor;
    ComPtr<ID3D11ShaderResourceView> myTexture;
    ComPtr<ID3D11ShaderResourceView> myReflectionTexture;
    ConstantBuffer<CB_VS_StandardShader> myCBVSVertexShader;
    ConstantBuffer<CB_PS_PBRShader> myCBPSPixelShader;
    TransformComponent* myTransform;
    //std::shared_ptr<std::vector<D3D11_INPUT_ELEMENT_DESC>> myLayout;
};

