#pragma once
#include "Component.h"

class TransformComponent : public Component
{
public:
    XMFLOAT3 myPosition;
    XMFLOAT3 myRotation;
    XMFLOAT3 myScale;

    TransformComponent()
        : myPosition(XMFLOAT3(0.0f, 0.0f, 0.0f)),
        myRotation(XMFLOAT3(0.0f, 0.0f, 0.0f)),
        myScale(XMFLOAT3(1.0f, 1.0f, 1.0f))
    {
    }
};