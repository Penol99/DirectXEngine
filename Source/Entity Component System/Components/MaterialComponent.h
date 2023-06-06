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
    void Init() override;
    void Render();
    void SetColor(const DirectX::XMFLOAT4& aColor);
    void SetTexture(std::wstring aTexturePath);
    void SetVertexShader(std::wstring aVertexShaderPath);
    void SetPixelShader(std::wstring aPixelShaderPath);
    void SetReflectionTexture(std::wstring aTexturePath);
    void RenderImGui() override;

    void Serialize(json& serializedObject) const override;
    void Deserialize(const json& serializedObject) override;
private:
    VertexShader myVertexShader;
    PixelShader myPixelShader;
    DirectX::XMFLOAT4 myColor;
    ComPtr<ID3D11ShaderResourceView> myTexture;
    ComPtr<ID3D11ShaderResourceView> myReflectionTexture;
    ConstantBuffer<CB_VS_StandardShader> myCBVSVertexShader;
    ConstantBuffer<CB_PS_PBRShader> myCBPSPixelShader;
    TransformComponent* myTransform;
    std::string myVertexShaderPath;
    std::string myPixelShaderPath;
    std::string myTexturePath;
    std::string myReflectionPath;
    //std::shared_ptr<std::vector<D3D11_INPUT_ELEMENT_DESC>> myLayout;
};

