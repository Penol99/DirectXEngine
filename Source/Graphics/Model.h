//// Model.h
//#pragma once
//#include <iostream>
//#include <vector>
//#include <../assimp/Importer.hpp>
//#include <../assimp/scene.h>
//#include <../assimp/postprocess.h>
//#include "Mesh.h"
//#include "ConstantBuffer.h"
//#include "Camera.h"
//class Timer;
//class Model
//{
//public:
//    Model();
//    Model(const Model& other);
//    bool Init(ComPtr<ID3D11Device>& aDevice, ComPtr<ID3D11DeviceContext>& aDeviceContext, const std::string& filePath, const std::wstring aTexturePath, Camera& aCamera, std::wstring& aVertexShaderPath, std::wstring& aPixelShaderPath);
//    void Render(ID3D11DeviceContext* aDeviceContext, Timer& aTimer);
//
//    // New member variables for rotation and position
//    DirectX::XMFLOAT3 myRotationAngles = { 0.0f, 0.0f, 0.0f };
//    DirectX::XMFLOAT3 myPosition = { 0.0f, 0.0f, 0.0f };
//    DirectX::XMFLOAT3 myScale = { 1.0f, 1.0f, 1.0f };
//    void TranslatePosition(DirectX::XMFLOAT3 aPos);
//    void TranslateRotation(DirectX::XMFLOAT3 aRot);
//    void SetPosition(DirectX::XMFLOAT3 aPos);
//    void SetRotation(DirectX::XMFLOAT3 aRot);
//    void SetScale(DirectX::XMFLOAT3 aScale);
//    XMFLOAT3 GetPosition();
//    XMFLOAT3 GetRotation();
//    void SetName(std::string aName);
//    std::string GetName();
//private:
//    std::vector<Mesh> myMeshes;
//    std::wstring myTexturePath;
//    ConstantBuffer<CB_VS_StandardShader> myCBVSVertexShader;
//    ConstantBuffer<CB_PS_PBRShader> myCBPSPixelShader;
//    Camera* myCamera = nullptr;
//    void ProcessNode(const aiNode* node, const aiScene* scene, ComPtr<ID3D11Device>& aDevice, std::wstring& aVertexShaderPath, std::wstring& aPixelShaderPath);
//    std::string myName;
//
//};