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
    //std::vector<std::shared_ptr<Mesh>> meshes;
    void Init();
    void LoadModel(std::string aPath);
    void Render();
    void RenderImGui();
    void Serialize(json& serializedObject) const override;
    void Deserialize(const json& serializedObject) override;

private:
    void ProcessNode(const aiNode* node, const aiScene* scene);
    std::vector<Mesh> myMeshes;
    std::string myModelPath;

};
