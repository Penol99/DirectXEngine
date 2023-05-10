// Model.h
#pragma once
#include <iostream>
#include <vector>
//#include "../../Include/assimp/Importer.hpp"
//#include "../../Include/assimp/scene.h"
//#include "../../Include/assimp/postprocess.h"
#include <../assimp/Importer.hpp>
#include <../assimp/scene.h>
#include <../assimp/postprocess.h>
#include "Mesh.h"
#include "ConstantBuffer.h"
#include "Camera.h"
class Model
{
public:
    Model();
    Model(const Model& other);
    bool Init(ComPtr<ID3D11Device>& aDevice, ComPtr<ID3D11DeviceContext>& aDeviceContext, const std::string& filePath, const std::wstring& aTexturePath, Camera& aCamera);
    void Render(ID3D11DeviceContext* aDeviceContext);

    // New member variables for rotation and position
    DirectX::XMFLOAT3 mRotationAngles = { 0.0f, 0.0f, 0.0f };
    DirectX::XMFLOAT3 mPosition = { 0.0f, 0.0f, 0.0f };
    void TranslatePosition(DirectX::XMFLOAT3 aPos);
    void TranslateRotation(DirectX::XMFLOAT3 aRot);
    void SetPosition(DirectX::XMFLOAT3 aPos);
    void SetRotation(DirectX::XMFLOAT3 aRot);
    XMFLOAT3 GetPosition();
    XMFLOAT3 GetRotation();
    void SetName(std::string aName);
    std::string GetName();
private:
    std::vector<Mesh> mMeshes;
    std::wstring mTexturePath;
    ConstantBuffer<CB_VS_VertexShader> mCBVSVertexShader;
    ConstantBuffer<CB_PS_PixelShader> mCBPSPixelShader;
    Camera* myCamera;
    void ProcessNode(const aiNode* node, const aiScene* scene, ComPtr<ID3D11Device>& aDevice);
    std::string myName;
};