#include "ModelComponent.h"
#include "../GameObject.h"
#include <filesystem>
void ModelComponent::Init()
{
	myType = eComponentType::Model;
}

void ModelComponent::LoadModel(std::string aPath)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(aPath.c_str(), aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		ErrorLog::Log("Failed to load model file: ");
		ErrorLog::Log(importer.GetErrorString());
		return;
	}

	ProcessNode(scene->mRootNode, scene);
}

void ModelComponent::Render()
{
	for (Mesh& mesh : myMeshes)
	{

		mesh.Render(myGameObject->myDeviceContext.Get());
	}
}

void ModelComponent::RenderImGui()
{
	// Render base Component ImGui properties
	Component::RenderImGui();

	ImGui::Separator();

	ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Model Component");

	ImGui::Indent();

	ImGui::Text("Mesh Path:");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(200);
	if (ImGui::Button("Browse##Model"))
	{
		// Save the current working directory
		auto oldCurrentPath = std::filesystem::current_path();

		// Open file dialog to select the vertex shader path
		OPENFILENAMEA ofn;
		char fileName[MAX_PATH] = "";

		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;
		ofn.lpstrFilter = "FBX File (*.fbx)\0*.fbx\0All Files (*.*)\0*.*\0";
		ofn.lpstrFile = fileName;
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

		if (GetOpenFileNameA(&ofn))
		{
			// Store the selected vertex shader path in myVertexShaderPath variable
			myModelPath = ofn.lpstrFile;
			LoadModel(myModelPath);
		}

		// Restore the original working directory
		std::filesystem::current_path(oldCurrentPath);
	}

}

void ModelComponent::Serialize(json& serializedObject) const
{
	Component::Serialize(serializedObject);

	serializedObject["modelPath"] = StringConverter::FormatForJSONPath(std::string("Bin"), myModelPath);

	// Serialize meshes if needed...
}

void ModelComponent::Deserialize(const json& serializedObject)
{
	Component::Deserialize(serializedObject);

	myModelPath = serializedObject["modelPath"].get<std::string>();
	LoadModel(myModelPath);
	// Deserialize meshes if needed...
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
