#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "../../PropertyDefines.h"
#include "../../ImGui/imgui.h"
using namespace DirectX;
class GameObject;
class Component
{
public:
    virtual ~Component() {}
    virtual void Init() {};
    virtual void Render(){};
    virtual void RenderImGui(){};
    GameObject* myGameObject;
};


