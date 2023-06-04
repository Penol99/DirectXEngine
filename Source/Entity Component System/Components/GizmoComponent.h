#pragma once
#include "Component.h"
#include "../../Ray.h"
class GizmoComponent : public Component
{
public:
    GizmoComponent()
    {
        // Initialize your gizmo data here
    }

    ~GizmoComponent()
    {
        // Clean up your gizmo data here
    }

    void Render() override
    {
        // Render your gizmo here
    }

    bool HandleMouse(int mouseButton, int action, float mouseX, float mouseY)
    {
        // Convert mouse coordinates to ray and do ray-gizmo intersection tests
        // If intersection is detected, set the active axis and return true
        // Return false if no intersection is detected
        return false;
    }

    void TranslateAlongRay(Ray& ray, float amount)
    {
        // Translate parent GameObject along the ray
    }

    void RotateAroundRay(Ray& ray, float amount)
    {
        // Rotate parent GameObject around the ray
    }

private:
    // Gizmo mesh data
    // ...
    // Other necessary data
    // ...
};