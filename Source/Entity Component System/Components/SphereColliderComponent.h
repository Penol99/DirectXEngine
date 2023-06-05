#pragma once
#include "ColliderComponent.h"
class SphereColliderComponent : public ColliderComponent
{
public:
    SphereColliderComponent(DirectX::XMVECTOR center, float radius)
        : center(center), radius(radius)
    {
    }

    SphereColliderComponent()
        : center(0), radius(10)
    {
    }
    bool Intersects(const Ray& ray) const override;
#ifdef DRAW_COLLIDERS
    void Render() override;
#endif
private:
    DirectX::XMVECTOR center; // Center of the sphere
    float radius;
    // Inherited via ColliderComponent
    // Radius of the sphere
};
