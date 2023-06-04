//#include "MaterialManager.h"
//
//int MaterialManager::CreateMaterial(ComPtr<ID3D11Device>& device, const std::wstring& vertexShaderPath, const std::wstring& pixelShaderPath)
//{
//    Material material;
//    if (!material.Init(device, vertexShaderPath, pixelShaderPath))
//        return -1;
//
//    myMaterials.push_back(material);
//    return static_cast<int>(myMaterials.size()) - 1;
//}
//
//Material* MaterialManager::GetMaterial(int index)
//{
//    if (index >= 0 && index < static_cast<int>(myMaterials.size()))
//        return &myMaterials[index];
//
//    return nullptr;
//}
//
//void MaterialManager::BindMaterial(int index, ID3D11DeviceContext* deviceContext)
//{
//    Material* material = GetMaterial(index);
//    if (material)
//        material->Bind(deviceContext);
//}