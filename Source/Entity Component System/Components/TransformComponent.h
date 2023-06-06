#pragma once
#include "Component.h"

class TransformComponent : public Component
{
public:
    XMFLOAT3 myPosition;
    XMFLOAT3 myRotation;
    XMFLOAT3 myScale;
    TransformComponent* myParent;
    void Init();
    void RenderImGui() override;
    TransformComponent()
        : myPosition(XMFLOAT3(0.0f, 0.0f, 0.0f)),
        myRotation(XMFLOAT3(0.0f, 0.0f, 0.0f)),
        myScale(XMFLOAT3(1.0f, 1.0f, 1.0f)),
        myParent(nullptr)
    {
    }

    void Serialize(json& serializedObject) const override;
    void Deserialize(const json& serializedObject) override;
};