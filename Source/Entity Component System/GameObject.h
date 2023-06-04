#pragma once
#include "Components/TransformComponent.h"
#include "Components/ModelComponent.h"
#include "../Graphics/Camera.h"
#include "../Timer.h"
#include <wrl/client.h>


using Microsoft::WRL::ComPtr;
class GameObject
{
public:
    template <typename T>
    T* AddComponent()
    {
        static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");

        T* object = new T();
        object->myGameObject = this;
        components.push_back(object);
        return object;
    }

    template <typename T>
    T* GetComponent()
    {
        for (Component* component : components)
        {
            if (T* castComponent = dynamic_cast<T*>(component))
            {
                return castComponent;
            }
        }
        return nullptr; 
    }

    ~GameObject()
    {
        for (auto o : components)
        {
            delete o;
        }
    };

    Camera* myCamera;
    Timer* myTimer;
    TransformComponent* myTransform;
    ComPtr<ID3D11DeviceContext> myDeviceContext;
    ComPtr<ID3D11Device> myDevice;
    void Init(Timer* aTimer, Camera* aCamera, ComPtr<ID3D11Device>& aDevice, ComPtr<ID3D11DeviceContext>& aDeviceContext)
    {
        myTransform = AddComponent<TransformComponent>();
        myTimer = aTimer;
        myCamera = aCamera;
        myDevice = aDevice;
        myDeviceContext = aDeviceContext;

    }
    void Render()
    {
        for (auto o : components)
        {
            o->Render();
        }
    }
private:
    std::vector<Component*> components;
};