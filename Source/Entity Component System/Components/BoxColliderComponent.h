#pragma once
#include "ColliderComponent.h"

class BoxColliderComponent : public ColliderComponent
{
public:
    BoxColliderComponent()
        : myCenter(), myExtents(), myHasInitialized(false)
    {
        SetExtents(XMFLOAT3(10, 10, 10));
        SetCenter(XMFLOAT3(0, 0.5, 0));
    }

    void Init() override;
    bool Intersects(const Ray& ray) const override;
    DirectX::XMFLOAT3 GetExtents();
    DirectX::XMFLOAT3 GetCenter();
    void RenderImGui() override;
    void SetExtents(XMFLOAT3 someExtents);
    void SetCenter(XMFLOAT3 aCenter);


#ifdef DRAW_COLLIDERS
    void Render() override;
#endif
private:
    DirectX::XMVECTOR myCenter; // Center of the AABB box
    DirectX::XMVECTOR myExtents;
    bool myHasInitialized;
};

