#include "Model.h"

Model::Model(bool myShouldDrawImgui)
	:myShouldDrawImgui(myShouldDrawImgui)
{
	myPosition.x = 0;
	myPosition.y = 0;
	myPosition.z = 0;
}

Model::Model(const Model& other)
{
	myPosition = other.myPosition;
	myRotationAngles = other.myRotationAngles;
	myMeshes = other.myMeshes;
	myTexturePath = other.myTexturePath;
	myCamera = other.myCamera;
	myCBVSVertexShader = other.myCBVSVertexShader;
	myCBPSPixelShader = other.myCBPSPixelShader;
	myName = other.myName;
	myShouldDrawImgui = other.myShouldDrawImgui;
	for (const auto& mesh : other.myMeshes)
	{
		myMeshes.push_back(Mesh(mesh));
	}
}

bool Model::Init(ComPtr<ID3D11Device>& aDevice, ComPtr<ID3D11DeviceContext>& aDeviceContext, const std::string& filePath, const std::wstring& aTexturePath, Camera& aCamera)
{

	size_t lastSlash = filePath.find_last_of("/\\");
	if (lastSlash != std::string::npos)
	{
		myName = filePath.substr(lastSlash + 1);
	}
	else
	{
		myName = filePath;
	}
	myCamera = &aCamera;
	myTexturePath = aTexturePath;
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePath.c_str(), aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		ErrorLog::Log("Failed to load model file: ");
		ErrorLog::Log(importer.GetErrorString());
		return false;
	}

	ProcessNode(scene->mRootNode, scene, aDevice);

	
	HRESULT hr = myCBVSVertexShader.Init(aDevice.Get(), aDeviceContext.Get());
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "failed creating constant buffer.");
		return false;
	}
	hr = myCBPSPixelShader.Init(aDevice.Get(), aDeviceContext.Get());
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "failed creating constant buffer.");
		return false;
	}

	return true;
}

void Model::Render(ID3D11DeviceContext* aDeviceContext)
{

	static float worldScale[3] = { .5f,.5f,.5f };
	static float worldTranslationOffset[3] = { 0,0,0 };
	XMMATRIX scale = XMMatrixScaling(worldScale[0], worldScale[1], worldScale[2]);
	XMMATRIX translationOffset = XMMatrixTranslation(worldTranslationOffset[0], worldTranslationOffset[1], worldTranslationOffset[2]);
	XMMATRIX world = scale * translationOffset;

	myCBVSVertexShader.myData.modelPosition = myPosition;
	myCBVSVertexShader.myData.modelRotation = myRotationAngles;
	myCBVSVertexShader.myData.worldMatrix = world * myCamera->GetViewMatrix() * myCamera->GetProjectionMatrix();
	myCBVSVertexShader.myData.worldMatrix = XMMatrixTranspose(myCBVSVertexShader.myData.worldMatrix);


	aDeviceContext->UpdateSubresource(myCBVSVertexShader.Get(), 0, nullptr, &myCBVSVertexShader.myData, 0, 0);
	myCBVSVertexShader.ApplyChanges();
	aDeviceContext->VSSetConstantBuffers(0, 1, myCBVSVertexShader.GetAddressOf());
	
	myCBPSPixelShader.myData.alpha = 1.0f;
	myCBPSPixelShader.ApplyChanges();
	aDeviceContext->PSSetConstantBuffers(0, 1, myCBPSPixelShader.GetAddressOf());

	// Render each mesh
	for (Mesh& mesh : myMeshes)
	{
		mesh.Render(aDeviceContext);
	}
}

void Model::TranslatePosition(DirectX::XMFLOAT3 aPos)
{
	myPosition.x += aPos.x;
	myPosition.y += aPos.y;
	myPosition.z += aPos.z;
}
void Model::SetPosition(DirectX::XMFLOAT3 aPos)
{
	myPosition = aPos;
}
void Model::TranslateRotation(DirectX::XMFLOAT3 aRot)
{
	myRotationAngles.x += aRot.x;
	myRotationAngles.y += aRot.y;
	myRotationAngles.z += aRot.z;
}

void Model::SetRotation(DirectX::XMFLOAT3 aRot)
{
	myRotationAngles = aRot;
}

XMFLOAT3 Model::GetPosition()
{
	return myPosition;
}

XMFLOAT3 Model::GetRotation()
{
	return myRotationAngles;
}

void Model::SetName(std::string aName)
{
	myName = aName;
}

std::string Model::GetName()
{
	return myName;
}

bool Model::ShouldDrawImgui()
{
	return myShouldDrawImgui;
}

void Model::ProcessNode(const aiNode* node, const aiScene* scene, ComPtr<ID3D11Device>& aDevice)
{
	// Process mesh data
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		

		std::vector<Vertex> vertices;
		std::vector<DWORD> indices;
		
		// Process vertices
		for (unsigned int j = 0; j < mesh->mNumVertices; j++)
		{
			const aiVector3D& vertex = mesh->mVertices[j];
			const aiVector3D& uv = mesh->mTextureCoords[0][j];

			Vertex v(vertex.x, vertex.y, vertex.z, (float)uv.x, (float)uv.y);
			vertices.push_back(v);
		}

		// Process indices
		for (unsigned int j = 0; j < mesh->mNumFaces; j++)
		{
			const aiFace& face = mesh->mFaces[j];

			for (unsigned int k = 0; k < face.mNumIndices; k++)
			{
				indices.push_back(face.mIndices[k]);
			}
		}
		UINT numVertices = static_cast<UINT>(vertices.size());
		UINT numIndices = static_cast<UINT>(indices.size());
		Mesh newMesh;
		if (!newMesh.Init(aDevice, vertices, indices, numVertices, numIndices, myTexturePath))
		{
			ErrorLog::Log("Failed to initialize new mesh when processing node");
			return;
		}
		myMeshes.push_back(newMesh);

	}

	// Process child nodes recursively
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene, aDevice);
	}
}

