#include <DirectXMath.h>
#include "ConstantBuffer.h"

class DirectionalLight
{
public:
    ~DirectionalLight();

    inline static DirectionalLight* GetInstance()
    {
        if (myInstance == nullptr)
        {
            myInstance = new DirectionalLight();
        }
        return myInstance;
    }

    inline static void DestroyInstance()
    {
        delete myInstance;
    }

    inline static DirectX::XMFLOAT4 myAmbientColor;
    inline static DirectX::XMFLOAT4 myDiffuseColor;
    inline static DirectX::XMFLOAT3 myDirection;
    ID3D11Buffer* myDirectionalLightBuffer;
private:
    inline static DirectionalLight* myInstance;
    DirectionalLight()
    {
        // Set default light properties
        myAmbientColor = DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
        myDiffuseColor = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
        myDirection = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f);
        myDirectionalLightBuffer = nullptr;
    }
};


