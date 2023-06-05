#include "MaterialComponent.h"
#include "Component.h"
#include <DirectXMath.h>
#include <WICTextureLoader.h>
#include "../../Graphics/DirectionalLight.h"
#include "../GameObject.h"
bool MaterialComponent::Init(const std::wstring& aVertexShaderPath, const std::wstring& aPixelShaderPath)
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
		return false;
	}

	if (!myPixelShader.Init(myGameObject->myDevice, aPixelShaderPath))
	{
		return false;
	}

	HRESULT hr = myCBVSVertexShader.Init(myGameObject->myDevice.Get(), myGameObject->myDeviceContext.Get());
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "failed creating constant buffer.");
		return false;
	}
	hr = myCBPSPixelShader.Init(myGameObject->myDevice.Get(), myGameObject->myDeviceContext.Get());
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "failed creating constant buffer.");
		return false;
	}
	myTransform = myGameObject->GetComponent<TransformComponent>();
    return true;
}

void MaterialComponent::Render()
{
	myGameObject->myDeviceContext->IASetInputLayout(myVertexShader.GetInputLayout());
	myGameObject->myDeviceContext->VSSetShader(myVertexShader.GetShader(), NULL, 0);
	myGameObject->myDeviceContext->PSSetShader(myPixelShader.GetShader(), NULL, 0);


	myGameObject->myDeviceContext->PSSetShaderResources(0, 1, myTexture.GetAddressOf());
	myGameObject->myDeviceContext->PSSetShaderResources(3, 1, myReflectionTexture.GetAddressOf());

	static float worldTranslationOffset[3] = { 0,0,0 };
	DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(myTransform->myLocalScale.x, myTransform->myLocalScale.y, myTransform->myLocalScale.z);
	DirectX::XMMATRIX translationOffset = DirectX::XMMatrixTranslation(worldTranslationOffset[0], worldTranslationOffset[1], worldTranslationOffset[2]);
	DirectX::XMMATRIX world = scale * translationOffset;


	myCBVSVertexShader.myData.modelPosition = myTransform->myLocalPosition;
	myCBVSVertexShader.myData.modelRotation = myTransform->myLocalRotation;
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

void MaterialComponent::SetReflectionTexture(std::wstring aTexturePath)
{
	HRESULT hr = DirectX::CreateWICTextureFromFile(myGameObject->myDevice.Get(), aTexturePath.c_str(), nullptr, myReflectionTexture.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "failed creating texture from file.");

	}
}