#include "Model.h"

Model::Model()
{
	

}

bool Model::Init(ComPtr<ID3D11Device>& aDevice, const std::string& filePath, const std::wstring& aTexturePath)
{
	mTexturePath = aTexturePath;
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		ErrorLog::Log("Failed to load model file: ");
		ErrorLog::Log(importer.GetErrorString());
		return false;
	}

	ProcessNode(scene->mRootNode, scene, aDevice);

	return true;
}

void Model::Render(ID3D11DeviceContext* aDeviceContext)
{
	for (Mesh& mesh : mMeshes)
	{
		mesh.Render(aDeviceContext);
	}
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

