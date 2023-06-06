#include "MaterialComponent.h"
#include "Component.h"
#include <DirectXMath.h>
#include <WICTextureLoader.h>
#include "../../Graphics/DirectionalLight.h"
#include "../GameObject.h"
#include <filesystem>
void MaterialComponent::Init()
{
	HRESULT hr = myCBVSVertexShader.Init(myGameObject->myDevice.Get(), myGameObject->myDeviceContext.Get());
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "failed creating constant buffer.");
		return;
	}
	hr = myCBPSPixelShader.Init(myGameObject->myDevice.Get(), myGameObject->myDeviceContext.Get());
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "failed creating constant buffer.");
		return;
	}
	myTransform = myGameObject->GetComponent<TransformComponent>();
    myGameObject->myMaterial = this;
    myType = eComponentType::Material;
}

void MaterialComponent::Render()
{
    if (myVertexShader.GetBuffer() != NULL) 
    {
	    myGameObject->myDeviceContext->IASetInputLayout(myVertexShader.GetInputLayout());
	    myGameObject->myDeviceContext->VSSetShader(myVertexShader.GetShader(), NULL, 0);
    }
    if (myPixelShader.GetBuffer() != NULL)
    {
        myGameObject->myDeviceContext->PSSetShader(myPixelShader.GetShader(), NULL, 0);
    }

    if (myTexture != NULL)
    {
        myGameObject->myDeviceContext->PSSetShaderResources(0, 1, myTexture.GetAddressOf());
    }
    if (myReflectionTexture != NULL)
    {
        myGameObject->myDeviceContext->PSSetShaderResources(3, 1, myReflectionTexture.GetAddressOf());
    }

    DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(myTransform->myScale.x, myTransform->myScale.y, myTransform->myScale.z);
    DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationRollPitchYaw(myTransform->myRotation.x, myTransform->myRotation.y, myTransform->myRotation.z);
    DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(myTransform->myPosition.x, myTransform->myPosition.y, myTransform->myPosition.z);

    // Apply scaling, rotation and translation in order
    DirectX::XMMATRIX world = scale * rotation * translation;

    myCBVSVertexShader.myData.worldMatrix = world * myGameObject->myCamera->GetViewMatrix() * myGameObject->myCamera->GetProjectionMatrix();
    myCBVSVertexShader.myData.worldMatrix = DirectX::XMMatrixTranspose(myCBVSVertexShader.myData.worldMatrix);

    myCBVSVertexShader.ApplyChanges();
    myGameObject->myDeviceContext->VSSetConstantBuffers(0, 1, myCBVSVertexShader.GetAddressOf());

    myCBPSPixelShader.myData.cameraPosition = myGameObject->myCamera->GetPositionFloat3();
    myCBPSPixelShader.myData.direction = DirectionalLight::GetInstance()->myDirection;
    myCBPSPixelShader.myData.ambientColor = DirectionalLight::GetInstance()->myAmbientColor;
    myCBPSPixelShader.myData.diffuseColor = DirectionalLight::GetInstance()->myDiffuseColor;
    myCBPSPixelShader.ApplyChanges();
    myGameObject->myDeviceContext->PSSetConstantBuffers(0, 1, myCBPSPixelShader.GetAddressOf());


}

void MaterialComponent::SetColor(const DirectX::XMFLOAT4& aColor)
{
    myColor = aColor;
}

void MaterialComponent::SetTexture(std::wstring aTexturePath)
{
	HRESULT hr = DirectX::CreateWICTextureFromFile(myGameObject->myDevice.Get(), aTexturePath.c_str(), nullptr, myTexture.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "failed creating texture from file.");

	}
}

void MaterialComponent::SetVertexShader(std::wstring aVertexShaderPath)
{
    D3D11_INPUT_ELEMENT_DESC layout[3];

    layout[0].SemanticName = "POSITION";
    layout[0].SemanticIndex = 0;
    layout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    layout[0].InputSlot = 0;
    layout[0].AlignedByteOffset = 0;
    layout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    layout[0].InstanceDataStepRate = 0;

    layout[1].SemanticName = "NORMAL";
    layout[1].SemanticIndex = 0;
    layout[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    layout[1].InputSlot = 0;
    layout[1].AlignedByteOffset = 12;
    layout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    layout[1].InstanceDataStepRate = 0;

    layout[2].SemanticName = "TEXCOORD";
    layout[2].SemanticIndex = 0;
    layout[2].Format = DXGI_FORMAT_R32G32_FLOAT;
    layout[2].InputSlot = 0;
    layout[2].AlignedByteOffset = 24;
    layout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    layout[2].InstanceDataStepRate = 0;

    UINT numElements = ARRAYSIZE(layout);//(UINT)myLayout->size();//

    if (!myVertexShader.Init(myGameObject->myDevice, aVertexShaderPath, layout, numElements))
    {
        return;
    }
}

void MaterialComponent::SetPixelShader(std::wstring aPixelShaderPath)
{
    if (!myPixelShader.Init(myGameObject->myDevice, aPixelShaderPath))
    {
        return;
    }
}

void MaterialComponent::SetReflectionTexture(std::wstring aTexturePath)
{
	HRESULT hr = DirectX::CreateWICTextureFromFile(myGameObject->myDevice.Get(), aTexturePath.c_str(), nullptr, myReflectionTexture.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "failed creating texture from file.");

	}
}

void MaterialComponent::RenderImGui()
{
    // Render base Component ImGui properties
    Component::RenderImGui();

    ImGui::Separator();

    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Material Component");

    ImGui::Indent();

    // Vertex shader path
    ImGui::Text("Vertex Shader Path:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(200);
    if (ImGui::Button("Browse##VertexShader"))
    {
        // Save the current working directory
        auto oldCurrentPath = std::filesystem::current_path();

        // Open file dialog to select the vertex shader path
        OPENFILENAMEA ofn;
        char fileName[MAX_PATH] = "";

        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = NULL;
        ofn.lpstrFilter = "Compiled Shader Object (*.cso)\0*.cso\0All Files (*.*)\0*.*\0";
        ofn.lpstrFile = fileName;
        ofn.nMaxFile = MAX_PATH;
        ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

        if (GetOpenFileNameA(&ofn))
        {
            // Store the selected vertex shader path in myVertexShaderPath variable
            myVertexShaderPath = ofn.lpstrFile;
            SetVertexShader(StringConverter::StringToWide(myVertexShaderPath));
        }

        // Restore the original working directory
        std::filesystem::current_path(oldCurrentPath);
    }

    // Pixel shader path
    ImGui::Text("Pixel Shader Path:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(200);
    if (ImGui::Button("Browse##PixelShader"))
    {
        // Save the current working directory
        auto oldCurrentPath = std::filesystem::current_path();

        // Open file dialog to select the pixel shader path
        OPENFILENAMEA ofn;
        char fileName[MAX_PATH] = "";

        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = NULL;
        ofn.lpstrFilter = "Compiled Shader Object (*.cso)\0*.cso\0All Files (*.*)\0*.*\0";
        ofn.lpstrFile = fileName;
        ofn.nMaxFile = MAX_PATH;
        ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

        if (GetOpenFileNameA(&ofn))
        {
            // Store the selected pixel shader path in myPixelShaderPath variable
            myPixelShaderPath = ofn.lpstrFile;
            SetPixelShader(StringConverter::StringToWide(myPixelShaderPath));
        }
        std::filesystem::current_path(oldCurrentPath);

    }

    // Texture file path
    ImGui::Text("Texture File Path:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(200);
    if (ImGui::Button("Browse##Texture"))
    {
        // Save the current working directory
        auto oldCurrentPath = std::filesystem::current_path();

        // Open file dialog to select the texture file path
        OPENFILENAMEA ofn;
        char fileName[MAX_PATH] = "";

        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = NULL;
        ofn.lpstrFilter = "Image Files (*.png;*.jpg;*.bmp)\0*.png;*.jpg;*.bmp\0All Files (*.*)\0*.*\0";
        ofn.lpstrFile = fileName;
        ofn.nMaxFile = MAX_PATH;
        ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

        if (GetOpenFileNameA(&ofn))
        {
            // Store the selected texture file path in myTexturePath variable
            myTexturePath = ofn.lpstrFile;
            SetTexture(StringConverter::StringToWide(myTexturePath));
        }
        std::filesystem::current_path(oldCurrentPath);

    }

    // Reflection file path
    ImGui::Text("Reflection File Path:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(200);
    if (ImGui::Button("Browse##Reflection"))
    {
        // Save the current working directory
        auto oldCurrentPath = std::filesystem::current_path();

        // Open file dialog to select the reflection file path
        OPENFILENAMEA ofn;
        char fileName[MAX_PATH] = "";

        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = NULL;
        ofn.lpstrFilter = "Image Files (*.png;*.jpg;*.bmp)\0*.png;*.jpg;*.bmp\0All Files (*.*)\0*.*\0";
        ofn.lpstrFile = fileName;
        ofn.nMaxFile = MAX_PATH;
        ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

        if (GetOpenFileNameA(&ofn))
        {
            // Store the selected reflection file path in myReflectionPath variable
            myReflectionPath = ofn.lpstrFile;
            SetReflectionTexture(StringConverter::StringToWide(myReflectionPath));
        }
        std::filesystem::current_path(oldCurrentPath);

    }

    ImGui::Unindent();
}

void MaterialComponent::Serialize(json& serializedObject) const
{
    Component::Serialize(serializedObject);

    serializedObject["vertexShaderPath"] = myVertexShaderPath;
    serializedObject["pixelShaderPath"] = myPixelShaderPath;
    serializedObject["texturePath"] = myTexturePath;
    serializedObject["reflectionPath"] = myReflectionPath;
    // Serialize other properties if needed...
}

void MaterialComponent::Deserialize(const json& serializedObject)
{
    Component::Deserialize(serializedObject);

    myVertexShaderPath = serializedObject["vertexShaderPath"].get<std::string>();
    myPixelShaderPath = serializedObject["pixelShaderPath"].get<std::string>();
    myTexturePath = serializedObject["texturePath"].get<std::string>();
    myReflectionPath = serializedObject["reflectionPath"].get<std::string>();
    SetVertexShader(StringConverter::StringToWide(myVertexShaderPath));
    SetPixelShader(StringConverter::StringToWide(myPixelShaderPath));
    SetTexture(StringConverter::StringToWide(myTexturePath));
    SetReflectionTexture(StringConverter::StringToWide(myReflectionPath));
}
