#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "../../PropertyDefines.h"
using namespace DirectX;
class GameObject;
class Component
{
public:
    virtual ~Component() {}
    virtual void Render(){};
    GameObject* myGameObject;
};


