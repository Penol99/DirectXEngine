#include "Model.h"

Model::Model()
{
	mPosition.x = 0;
	mPosition.y = 0;
	mPosition.z = 0;
}

Model::Model(const Model& other)
{
	mPosition = other.mPosition;
	mRotationAngles = other.mRotationAngles;
	mMeshes = other.mMeshes;
	mTexturePath = other.mTexturePath;
	myCamera = other.myCamera;
	mCBVSVertexShader = other.mCBVSVertexShader;
	mCBPSPixelShader = other.mCBPSPixelShader;
	myName = other.myName;
	for (const auto& mesh : other.mMeshes)
	{
		mMeshes.push_back(Mesh(mesh));
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
	mTexturePath = aTexturePath;
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePath.c_str(), aiProcess_Triangulate);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		ErrorLog::Log("Failed to load model file: ");
		ErrorLog::Log(importer.GetErrorString());
		return false;
	}

	ProcessNode(scene->mRootNode, scene, aDevice);

	
	HRESULT hr = mCBVSVertexShader.Init(aDevice.Get(), aDeviceContext.Get());
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "fucked up creating constant buffer.");
		return false;
	}
	hr = mCBPSPixelShader.Init(aDevice.Get(), aDeviceContext.Get());
	if (FAILED(hr))
	{
		ErrorLog::Log(hr, "fucked up creating constant buffer.");
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

	mCBVSVertexShader.mData.gModelPosition = mPosition;
	mCBVSVertexShader.mData.gModelRotation = mRotationAngles;
	mCBVSVertexShader.mData.worldMatrix = world * myCamera->GetViewMatrix() * myCamera->GetProjectionMatrix();
	mCBVSVertexShader.mData.worldMatrix = XMMatrixTranspose(mCBVSVertexShader.mData.worldMatrix);


	aDeviceContext->UpdateSubresource(mCBVSVertexShader.Get(), 0, nullptr, &mCBVSVertexShader.mData, 0, 0);
	mCBVSVertexShader.ApplyChanges();
	aDeviceContext->VSSetConstantBuffers(0, 1, mCBVSVertexShader.GetAddressOf());
	
	mCBPSPixelShader.mData.alpha = 1.0f;
	mCBPSPixelShader.ApplyChanges();
	aDeviceContext->PSSetConstantBuffers(0, 1, mCBPSPixelShader.GetAddressOf());

	// Render each mesh
	for (Mesh& mesh : mMeshes)
	{
		mesh.Render(aDeviceContext);
	}
}

void Model::TranslatePosition(DirectX::XMFLOAT3 aPos)
{
	mPosition.x += aPos.x;
	mPosition.y += aPos.y;
	mPosition.z += aPos.z;
}
void Model::SetPosition(DirectX::XMFLOAT3 aPos)
{
	mPosition = aPos;
}
void Model::TranslateRotation(DirectX::XMFLOAT3 aRot)
{
	mRotationAngles.x += aRot.x;
	mRotationAngles.y += aRot.y;
	mRotationAngles.z += aRot.z;
}

void Model::SetRotation(DirectX::XMFLOAT3 aRot)
{
	mRotationAngles = aRot;
}

XMFLOAT3 Model::GetPosition()
{
	return mPosition;
}

XMFLOAT3 Model::GetRotation()
{
	return mRotationAngles;
}

void Model::SetName(std::string aName)
{
	myName = aName;
}

std::string Model::GetName()
{
	return myName;
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

			Vertex v(vertex.x, vertex.y, vertex.z, uv.x, uv.y);
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

		Mesh newMesh;
		if (!newMesh.Init(aDevice, vertices, indices, mTexturePath))
		{
			ErrorLog::Log("Failed to initialize new mesh when processing node");
			return;
		}
		mMeshes.push_back(newMesh);

	}

	// Process child nodes recursively
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene, aDevice);
	}
}

