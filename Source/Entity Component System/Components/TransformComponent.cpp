#include "TransformComponent.h"

void TransformComponent::RenderImGui()
{
    // Display the local position, rotation, and scale values
    ImGui::Text("Local Position");
    ImGui::DragFloat3("##LocalPosition", &myLocalPosition.x, 0.1f);

    ImGui::Text("Local Rotation");
    ImGui::DragFloat3("##LocalRotation", &myLocalRotation.x, 0.1f);

    ImGui::Text("Local Scale");
    ImGui::DragFloat3("##LocalScale", &myLocalScale.x, 0.1f);

    // Display the global position, rotation, and scale values
    ImGui::Text("Global Position");
    ImGui::DragFloat3("##GlobalPosition", &myPosition.x, 0.1f, -FLT_MAX, FLT_MAX, "%.3f", ImGuiSliderFlags_AlwaysClamp);

    ImGui::Text("Global Rotation");
    ImGui::DragFloat3("##GlobalRotation", &myRotation.x, 0.1f, -FLT_MAX, FLT_MAX, "%.3f", ImGuiSliderFlags_AlwaysClamp);

    ImGui::Text("Global Scale");
    ImGui::DragFloat3("##GlobalScale", &myScale.x, 0.1f, -FLT_MAX, FLT_MAX, "%.3f", ImGuiSliderFlags_AlwaysClamp);

    // Display the parent information
    ImGui::Text("Parent");
    if (myParent)
    {
        ImGui::Text("parent");//myParent->GetName().c_str());
    }
    else
    {
        ImGui::Text("None");
    }
}
