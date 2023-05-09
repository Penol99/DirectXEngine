#pragma once
#include <iostream>
#include <vector>
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
class Model
{

public:
    Model();
    bool Init(ComPtr<ID3D11Device>& aDevice, const std::string& filePath, const std::wstring& aTexturePath);
    void Render(ID3D11DeviceContext* aDeviceContext);

private:  
    std::vector<Mesh> mMeshes;
    std::wstring mTexturePath;
    void ProcessNode(const aiNode* node, const aiScene* scene, ComPtr<ID3D11Device>& aDevice);

};

