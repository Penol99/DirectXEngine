#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "../../PropertyDefines.h"
#include "../../ImGui/imgui.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

using namespace DirectX;
class GameObject;
class Component
{
public:
    enum class eComponentType
    {
        Transform,
        Material,
        Model,
        Collider,
        BoxCollider,
        SphereCollider,
        Gizmo
    };

    virtual ~Component() {}
    virtual void Init() {};
    virtual void Render(){};
    virtual void RenderImGui(){};
    virtual void Serialize(json& serializedObject) const {};
    virtual void Deserialize(const json& serializedObject) {};
    const eComponentType GetType() const { return myType; }
    
    eComponentType myType;
    GameObject* myGameObject;
};


