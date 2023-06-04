#pragma once
#include "ColliderComponent.h"

class BoxColliderComponent : public ColliderComponent
{
public:
    BoxColliderComponent()
        : myCenter(), myExtents(), myHasInitialized(false)
    {
    }

    bool Intersects(const Ray& ray) const override;

    DirectX::XMFLOAT3 GetExtents();
    DirectX::XMFLOAT3 GetCenter();
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

