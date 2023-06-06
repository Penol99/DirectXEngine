#pragma once
#include "Components/TransformComponent.h"
#include "Components/ModelComponent.h"
#include "Components/MaterialComponent.h"
#include "../Graphics/Camera.h"
#include "../Timer.h"
#include <wrl/client.h>
#include <vector>
using Microsoft::WRL::ComPtr;

class GameObject
{
public:
    GameObject() : myParent(nullptr), myName("Game Object") {}

    template <typename T>
    T* AddComponent()
    {
        static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");

        T* object = new T();
        object->myGameObject = this;
        myComponents.push_back(object);
        return object;
    }
    void AddComponent(Component* aComponent)
    {
        
        aComponent->myGameObject = this;
        myComponents.push_back(aComponent);
    }
    template <typename T>
    T* GetComponent()
    {
        for (Component* component : myComponents)
        {
            if (T* castComponent = dynamic_cast<T*>(component))
            {
                return castComponent;
            }
        }
        return nullptr;
    }

    void SetParent(GameObject* parent)
    {
        myParent = parent;
    }

    GameObject* GetParent() const
    {
        return myParent;
    }

    const std::vector<GameObject*>& GetChildren() const
    {
        return myChildren;
    }

    void AddChild(GameObject* child)
    {
        child->SetParent(this);
        myChildren.push_back(child);
    }

    void RemoveChild(GameObject* child)
    {
        auto it = std::find(myChildren.begin(), myChildren.end(), child);
        if (it != myChildren.end())
        {
            (*it)->SetParent(nullptr);
            myChildren.erase(it);
        }
    }

    ~GameObject()
    {
        for (auto o : myComponents)
        {
            delete o;
        }
    };

    Camera* myCamera;
    Timer* myTimer;
    TransformComponent* myTransform;
    std::string myName;
    MaterialComponent* myMaterial;
    ComPtr<ID3D11DeviceContext> myDeviceContext;
    ComPtr<ID3D11Device> myDevice;

    void Init(Timer* aTimer, Camera* aCamera, ComPtr<ID3D11Device>& aDevice, ComPtr<ID3D11DeviceContext>& aDeviceContext)
    {
        
        if (myParent != nullptr)
        {
            myTransform->myParent = myParent->GetComponent<TransformComponent>();
        }
        myTimer = aTimer;
        myCamera = aCamera;
        myDevice = aDevice;
        myDeviceContext = aDeviceContext;
    }

    void Render()
    {
        //Makes sure that the material is always rendered first, so that shaders are set up correctly
        if (myMaterial != NULL)
        {
            myMaterial->Render();
        }
        for (auto o : myComponents)
        {
            if (o == myMaterial)
            {
                continue;
            }
            o->myGameObject = this;
            o->Render();
        }

        for (auto child : myChildren)
        {
            child->Render();
        }
    }

    std::vector<Component*> myComponents;
    GameObject* myParent;
    std::vector<GameObject*> myChildren;
private:

};