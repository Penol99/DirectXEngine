#include "SceneSerializer.h"
#include <fstream>
#include <iostream>
#include "Entity Component System/ComponentFactory.h"
#include "Graphics/Graphics.h"
void SceneSerializer::SaveScene(const std::vector<GameObject*>& gameObjects, const std::string& filePath)
{
    json sceneData;

    // Serialize each GameObject
    json gameObjectsData;
    for (const GameObject* gameObject : gameObjects)
    {
        json gameObjectData;
        SceneSerializer::SerializeGameObject(gameObject, gameObjectData);
        gameObjectsData.push_back(gameObjectData);
    }
    sceneData["gameObjects"] = gameObjectsData;

    // Save scene data to file
    std::ofstream outputFile(filePath);
    if (outputFile.is_open())
    {
        outputFile << sceneData.dump(4); // Indent JSON data with 4 spaces
        outputFile.close();
        std::cout << "Scene saved successfully." << std::endl;
    }
    else
    {
        std::cerr << "Failed to save scene." << std::endl;
    }
}

std::vector<GameObject*> SceneSerializer::LoadScene(const std::string& filePath, Graphics* aGfx)
{
    std::vector<GameObject*> loadedGameObjects;

    // Load scene data from file
    std::ifstream inputFile(filePath);
    if (inputFile.is_open())
    {
        json sceneData;
        inputFile >> sceneData;

        // Deserialize each GameObject
        const json& gameObjectsData = sceneData["gameObjects"];
        for (const auto& gameObjectData : gameObjectsData)
        {
            GameObject* gameObject = SceneSerializer::DeserializeGameObject(gameObjectData,aGfx);
            loadedGameObjects.push_back(gameObject);
        }

        std::cout << "Scene loaded successfully." << std::endl;
    }
    else
    {
        std::cerr << "Failed to load scene." << std::endl;
    }

    return loadedGameObjects;
}

void SceneSerializer::SerializeGameObject(const GameObject* gameObject, json& serializedObject)
{
    if (!gameObject)
    {
        std::cerr << "SerializeGameObject - GameObject is null\n";
        return;
    }

    serializedObject["name"] = gameObject->myName;

    json componentsData;
    for (const Component* component : gameObject->myComponents)
    {
        if (!component)
        {
            std::cerr << "SerializeGameObject - Found null component in " << gameObject->myName << "\n";
            continue;
        }

        json componentData;
        componentData["type"] = ComponentFactory::TypeToString(component->GetType());

        component->Serialize(componentData);

        componentsData.push_back(componentData);
    }
    serializedObject["components"] = componentsData;

    json childrenData;
    for (const GameObject* child : gameObject->GetChildren())
    {
        if (!child)
        {
            std::cerr << "SerializeGameObject - Found null child in " << gameObject->myName << "\n";
            continue;
        }

        json childData;
        SceneSerializer::SerializeGameObject(child, childData);
        childrenData.push_back(childData);
    }
    serializedObject["children"] = childrenData;
}

GameObject* SceneSerializer::DeserializeGameObject(const json& serializedObject, Graphics* aGfx)
{
    if (serializedObject.is_null())
    {
        std::cerr << "DeserializeGameObject - serializedObject is null\n";
        return nullptr;
    }

    GameObject* gameObject = new GameObject();
    gameObject->Init(aGfx->myTimer, &aGfx->myCamera, aGfx->myDevice, aGfx->myDeviceContext);

    if (serializedObject.contains("name"))
        gameObject->myName = serializedObject["name"].get<std::string>();

    if (serializedObject.contains("components"))
    {
        const json& componentsData = serializedObject["components"];
        for (const auto& componentData : componentsData)
        {
            if (componentData.contains("type"))
            {
                std::string componentType = componentData["type"].get<std::string>();
                Component* component = ComponentFactory::CreateComponent(componentType);

                if (component)
                {
                    component->myGameObject = gameObject;
                    component->Init();
                    component->Deserialize(componentData);
                    gameObject->myComponents.push_back(component);
                }
                else
                {
                    std::cerr << "DeserializeGameObject - Failed to create component of type: " << componentType << "\n";
                }
            }
        }
    }

    if (serializedObject.contains("children"))
    {
        const json& childrenData = serializedObject["children"];
        for (const auto& childData : childrenData)
        {
            GameObject* childGameObject = SceneSerializer::DeserializeGameObject(childData, aGfx);
            if (childGameObject)
                gameObject->AddChild(childGameObject);
        }
    }

    return gameObject;
}