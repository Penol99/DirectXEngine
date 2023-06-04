#pragma once
#include "../../Graphics/Mesh.h"
#include "Component.h"
#include <memory>
#include <../assimp/Importer.hpp>
#include <../assimp/scene.h>
#include <../assimp/postprocess.h>
class ModelComponent : public Component
{
public:
    std::vector<std::shared_ptr<Mesh>> meshes;
    bool Init(const std::string& filePath);
    void Render();
    // Initialization method taking in mesh components...

private:
    void ProcessNode(const aiNode* node, const aiScene* scene);
    std::vector<Mesh> myMeshes;

};
