#include "SphereColliderComponent.h"

bool SphereColliderComponent::Intersects(const Ray& ray) const
{
    float outDistance;
    return ray.IntersectsSphere(center, radius, outDistance);
}

#ifdef DRAW_COLLIDERS
void SphereColliderComponent::Render()
{
}
#endif
