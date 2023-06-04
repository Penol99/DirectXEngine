#include "ModelComponent.h"
#include "../GameObject.h"
bool ModelComponent::Init(const std::string& filePath)
{
	//size_t lastSlash = filePath.find_last_of("/\\");
	//if (lastSlash != std::string::npos)
	//{
	//	myName = filePath.substr(lastSlash + 1);
	//}
	//else
	//{
	//	myName = filePath;
	//}
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePath.c_str(), aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		ErrorLog::Log("Failed to load model file: ");
		ErrorLog::Log(importer.GetErrorString());
		return false;
	}

	ProcessNode(scene->mRootNode, scene);
	return false;
}

void ModelComponent::Render()
{
	for (Mesh& mesh : myMeshes)
	{

		mesh.Render(myGameObject->myDeviceContext.Get());
	}
}


void ModelComponent::ProcessNode(const aiNode* node, const aiScene* scene)
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
			const aiVector3D& normal = mesh->mNormals[j];

			Vertex v(vertex.x, vertex.y, vertex.z, normal.x, normal.y, normal.z, uv.x, uv.y);
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
		if (!newMesh.Init(myGameObject->myDevice, vertices, indices, numVertices, numIndices))
		{
			ErrorLog::Log("Failed to initialize new mesh when processing node");
			return;
		}
		myMeshes.push_back(newMesh);
	}

	// Process child nodes recursively
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}
