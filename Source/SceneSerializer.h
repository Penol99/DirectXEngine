#pragma once

#include <vector>
#include <string>
#include "Entity Component System/GameObject.h"
class Graphics;
class SceneSerializer
{
public:
    static void SaveScene(const std::vector<GameObject*>& gameObjects, const std::string& filePath);
    static std::vector<GameObject*> LoadScene(const std::string& filePath, Graphics* aGfx);

    static void SerializeGameObject(const GameObject* gameObject, json& serializedObject);
    static GameObject* DeserializeGameObject(const json& serializedObject, Graphics* aGfx);
private:
};