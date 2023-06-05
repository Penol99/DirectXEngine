#pragma once
#include "Components/TransformComponent.h"
#include "Components/BoxColliderComponent.h"
#include "Components/ColliderComponent.h"
#include "Components/GizmoComponent.h"
#include "Components/MaterialComponent.h"
#include "Components/ModelComponent.h"
#include "Components/SphereColliderComponent.h"

class ComponentFactory
{
public:
    static Component* CreateComponent(const std::string& componentName)
    {
        if (componentName == "TransformComponent")
        {
            return new TransformComponent();
        }
        else if (componentName == "BoxColliderComponent")
        {
            return new BoxColliderComponent();
        }
        else if (componentName == "ColliderComponent")
        {
            return nullptr;
        }
        else if (componentName == "GizmoComponent")
        {
            return new GizmoComponent();
        }
        else if (componentName == "MaterialComponent")
        {
            return new MaterialComponent();
        }
        else if (componentName == "ModelComponent")
        {
            return new ModelComponent();
        }
        else if (componentName == "SphereColliderComponent")
        {
            return new SphereColliderComponent();
        }

        return nullptr;
    }
};