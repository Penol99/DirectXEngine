#include "TransformComponent.h"


void TransformComponent::Init()
{
    myType = eComponentType::Transform;
}

void TransformComponent::RenderImGui()
{

    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Transform");

    ImGui::Indent();

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
    

}

void TransformComponent::Serialize(json& serializedObject) const
{
    Component::Serialize(serializedObject);

    serializedObject["localPosition"]["x"] = myLocalPosition.x;
    serializedObject["localPosition"]["y"] = myLocalPosition.y;
    serializedObject["localPosition"]["z"] = myLocalPosition.z;

    serializedObject["localRotation"]["x"] = myLocalRotation.x;
    serializedObject["localRotation"]["y"] = myLocalRotation.y;
    serializedObject["localRotation"]["z"] = myLocalRotation.z;

    serializedObject["localScale"]["x"] = myLocalScale.x;
    serializedObject["localScale"]["y"] = myLocalScale.y;
    serializedObject["localScale"]["z"] = myLocalScale.z;

    serializedObject["position"]["x"] = myPosition.x;
    serializedObject["position"]["y"] = myPosition.y;
    serializedObject["position"]["z"] = myPosition.z;

    serializedObject["rotation"]["x"] = myRotation.x;
    serializedObject["rotation"]["y"] = myRotation.y;
    serializedObject["rotation"]["z"] = myRotation.z;

    serializedObject["scale"]["x"] = myScale.x;
    serializedObject["scale"]["y"] = myScale.y;
    serializedObject["scale"]["z"] = myScale.z;
}

void TransformComponent::Deserialize(const json& serializedObject)
{
    Component::Deserialize(serializedObject);

    myLocalPosition.x = serializedObject["localPosition"]["x"].get<float>();
    myLocalPosition.y = serializedObject["localPosition"]["y"].get<float>();
    myLocalPosition.z = serializedObject["localPosition"]["z"].get<float>();

    myLocalRotation.x = serializedObject["localRotation"]["x"].get<float>();
    myLocalRotation.y = serializedObject["localRotation"]["y"].get<float>();
    myLocalRotation.z = serializedObject["localRotation"]["z"].get<float>();

    myLocalScale.x = serializedObject["localScale"]["x"].get<float>();
    myLocalScale.y = serializedObject["localScale"]["y"].get<float>();
    myLocalScale.z = serializedObject["localScale"]["z"].get<float>();

    myPosition.x = serializedObject["position"]["x"].get<float>();
    myPosition.y = serializedObject["position"]["y"].get<float>();
    myPosition.z = serializedObject["position"]["z"].get<float>();

    myRotation.x = serializedObject["rotation"]["x"].get<float>();
    myRotation.y = serializedObject["rotation"]["y"].get<float>();
    myRotation.z = serializedObject["rotation"]["z"].get<float>();

    myScale.x = serializedObject["scale"]["x"].get<float>();
    myScale.y = serializedObject["scale"]["y"].get<float>();
    myScale.z = serializedObject["scale"]["z"].get<float>();
}
