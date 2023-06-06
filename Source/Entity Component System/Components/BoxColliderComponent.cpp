#include "BoxColliderComponent.h"
#include "../GameObject.h"

void BoxColliderComponent::Init()
{
	myType = eComponentType::BoxCollider;
}

bool BoxColliderComponent::Intersects(const Ray& ray) const
{
	float outDistance;
	XMVECTOR center = XMLoadFloat3(&myGameObject->myTransform->myPosition) + myCenter;
	return ray.IntersectsAABB(center, myExtents, outDistance);
}

DirectX::XMFLOAT3 BoxColliderComponent::GetExtents()
{
	XMFLOAT3 extents;
	XMStoreFloat3(&extents, myCenter);
	return extents;
}

DirectX::XMFLOAT3 BoxColliderComponent::GetCenter()
{
	XMFLOAT3 center;
	XMStoreFloat3(&center, myCenter);
	return center;
}

void BoxColliderComponent::RenderImGui()
{
	// Render base ColliderComponent ImGui properties
	ColliderComponent::RenderImGui();

	// Render BoxColliderComponent-specific properties

	ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Box Collider");

	ImGui::Indent();

	// Display the local center and extents values
	XMFLOAT3 center = XMFLOAT3(XMVectorGetX(myCenter), XMVectorGetY(myCenter), XMVectorGetZ(myCenter));
	XMFLOAT3 extents = XMFLOAT3(XMVectorGetX(myExtents), XMVectorGetY(myExtents), XMVectorGetZ(myExtents));

	ImGui::Text("Center");
	ImGui::DragFloat3("##LocalCenter", &center.x, 0.01f);
	myCenter = XMLoadFloat3(&center);

	ImGui::Text("Extents");
	ImGui::DragFloat3("##LocalExtents", &extents.x, 0.01f);
	myExtents = XMLoadFloat3(&extents);

	ImGui::Unindent();
}


void BoxColliderComponent::SetExtents(XMFLOAT3 someExtents)
{
	myExtents = XMLoadFloat3(&someExtents);
}
void BoxColliderComponent::SetCenter(XMFLOAT3 aCenter)
{
	myCenter = XMLoadFloat3(&aCenter);

}
#ifdef DRAW_COLLIDERS
void BoxColliderComponent::Render()
{

	if (!myHasInitialized)
	{
		// Calculate half-dimensions

		XMFLOAT3 center = XMFLOAT3(XMVectorGetX(myCenter), XMVectorGetY(myCenter), XMVectorGetZ(myCenter));
		XMFLOAT3 extents = XMFLOAT3(XMVectorGetX(myExtents), XMVectorGetY(myExtents), XMVectorGetZ(myExtents));


		// Define vertices for the eight corners of the box
		std::vector<Vertex> boxVertices =
		{
			Vertex(center.x - extents.x, center.y - extents.y, center.z - extents.z, 0, 0, 0, 0, 0),
			Vertex(center.x + extents.x, center.y - extents.y, center.z - extents.z, 0, 0, 0, 0, 0),
			Vertex(center.x + extents.x, center.y + extents.y, center.z - extents.z, 0, 0, 0, 0, 0),
			Vertex(center.x - extents.x, center.y + extents.y, center.z - extents.z, 0, 0, 0, 0, 0),
			Vertex(center.x - extents.x, center.y - extents.y, center.z + extents.z, 0, 0, 0, 0, 0),
			Vertex(center.x + extents.x, center.y - extents.y, center.z + extents.z, 0, 0, 0, 0, 0),
			Vertex(center.x + extents.x, center.y + extents.y, center.z + extents.z, 0, 0, 0, 0, 0),
			Vertex(center.x - extents.x, center.y + extents.y, center.z + extents.z, 0, 0, 0, 0, 0)
		};

		// Define indices for the twelve lines of the box
		std::vector<DWORD> boxIndices =
		{
			0, 1, 1, 2, 2, 3, 3, 0, // Bottom square
			4, 5, 5, 6, 6, 7, 7, 4, // Top square
			0, 4, 1, 5, 2, 6, 3, 7  // Connecting lines
		};

		HRESULT hr = myVertexBuffer.Init(myGameObject->myDevice.Get(), &boxVertices[0], static_cast<UINT>(boxVertices.size()));
		if (FAILED(hr))
		{
			ErrorLog::Log(hr, "Failed initializing box vertex buffer.");
			return;
		}

		hr = myIndexBuffer.Init(myGameObject->myDevice.Get(), &boxIndices[0], static_cast<UINT>(boxIndices.size()));
		if (FAILED(hr))
		{
			ErrorLog::Log(hr, "Failed initializing box indices buffer.");
			return;
		}

		hr = myConstantBuffer.Init(myGameObject->myDevice.Get(), myGameObject->myDeviceContext.Get());
		if (FAILED(hr))
		{
			ErrorLog::Log(hr, "Failed initializing constant buffer.");
			return;
		}

		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{"POSITION",0, DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0}
		};

		UINT numElements = ARRAYSIZE(layout);

		if (!myVertexShader.Init(myGameObject->myDevice, L"../bin/shaders/LineVertexShader.cso", layout, numElements))
		{
			ErrorLog::Log("Failed initializing box collider vertex shader");

		}

		if (!myPixelShader.Init(myGameObject->myDevice, L"../bin/shaders/LinePixelShader.cso"))
		{
			ErrorLog::Log("Failed initializing box collider pixel shader.");

		}
		myHasInitialized = true;
	}

	UINT stride = myVertexBuffer.GetStride();
	UINT offset = 0;

	// Calculate the world matrix from scale, rotation and translation
	DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(myGameObject->myTransform->myScale.x, myGameObject->myTransform->myScale.y, myGameObject->myTransform->myScale.z);
	DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationRollPitchYaw(myGameObject->myTransform->myRotation.x, myGameObject->myTransform->myRotation.y, myGameObject->myTransform->myRotation.z);
	DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(myGameObject->myTransform->myPosition.x + GetCenter().x,
		myGameObject->myTransform->myPosition.y + GetCenter().y,
		myGameObject->myTransform->myPosition.z + GetCenter().z);
	DirectX::XMMATRIX world = scale * rotation * translation;

	myConstantBuffer.myData.worldMatrix = world * myGameObject->myCamera->GetViewMatrix() * myGameObject->myCamera->GetProjectionMatrix();
	myConstantBuffer.myData.worldMatrix = XMMatrixTranspose(myConstantBuffer.myData.worldMatrix);

	myGameObject->myDeviceContext->UpdateSubresource(myConstantBuffer.Get(), 0, nullptr, &myConstantBuffer.myData, 0, 0);
	myConstantBuffer.ApplyChanges();
	myGameObject->myDeviceContext->VSSetConstantBuffers(1, 1, myConstantBuffer.GetAddressOf());

	myGameObject->myDeviceContext->IASetInputLayout(myVertexShader.GetInputLayout());
	myGameObject->myDeviceContext->VSSetShader(myVertexShader.GetShader(), NULL, 0);
	myGameObject->myDeviceContext->PSSetShader(myPixelShader.GetShader(), NULL, 0);

	myGameObject->myDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	myGameObject->myDeviceContext->IASetVertexBuffers(0, 1, myVertexBuffer.GetAddressOf(), &stride, &offset);
	myGameObject->myDeviceContext->IASetIndexBuffer(myIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	myGameObject->myDeviceContext->DrawIndexed(myIndexBuffer.GetBufferSize(), 0, 0);
}
#endif
