#include "ImGuiRenderer.h"
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_dx11.h"
#include "../ImGui/imgui_impl_win32.h"
#include "Graphics.h"
#include "../Entity Component System/ComponentFactory.h"
#include "../SceneSerializer.h"

void ImGuiRenderer::Init(Graphics* aGfx)
{
	myGfx = aGfx;
}

void ImGuiRenderer::Render(int aWidth, int aHeight)
{
    myWidth = aWidth;
    myHeight = aHeight;
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGuiID dockspace_id = ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
	RenderMainMenu();
    RenderCameraWindow(dockspace_id);
    RenderFileHierarchyWindow(dockspace_id);
    RenderComponentsWindow(dockspace_id);
    RenderSceneWindow();

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiRenderer::RenderMainMenu()
{

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save"))
			{
				std::string saveFilePath = OpenFileSaveDialog();
				if (!saveFilePath.empty())
				{
					SceneSerializer::SaveScene(myGfx->myGameObjects,saveFilePath);
				}
			}

			if (ImGui::MenuItem("Load"))
			{
				std::string loadFilePath = OpenFileLoadDialog();
				if (!loadFilePath.empty())
				{
					myGfx->myGameObjects = SceneSerializer::LoadScene(loadFilePath,myGfx);
				}
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

std::string ImGuiRenderer::OpenFileSaveDialog()
{
	auto oldCurrentPath = std::filesystem::current_path();

	OPENFILENAMEA ofn;
	CHAR szFile[MAX_PATH] = "";

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = "JSON Files (*.json)\0*.json\0All Files (*.*)\0*.*\0";
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrTitle = "Save Scene";
	ofn.Flags = OFN_OVERWRITEPROMPT;

	if (GetSaveFileNameA(&ofn) == TRUE)
	{
		std::filesystem::current_path(oldCurrentPath);
		return std::string(szFile);
	}
	

	return "";
}

std::string ImGuiRenderer::OpenFileLoadDialog()
{
	auto oldCurrentPath = std::filesystem::current_path();

	OPENFILENAMEA ofn;
	CHAR szFile[MAX_PATH] = "";

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = "JSON Files (*.json)\0*.json\0All Files (*.*)\0*.*\0";
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrTitle = "Load Scene";
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

	if (GetOpenFileNameA(&ofn) == TRUE)
	{
		std::filesystem::current_path(oldCurrentPath);
		return std::string(szFile);
	}
	return "";
}

void ImGuiRenderer::RenderCameraWindow(ImGuiID dockspace_id)
{
    ImGui::SetNextWindowDockID(dockspace_id, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(myWidth, myHeight), ImGuiCond_FirstUseEver);
    ImGui::Begin("Camera Window");

    ImVec2 windowSize = ImGui::GetWindowSize();

    float aspectRatio = (float)myWidth / (float)myHeight;

    float imageWidth, imageHeight;

    if (aspectRatio < 1.0f)
    {
        imageHeight = windowSize.y; // Take up the full window height
        imageWidth = imageHeight * aspectRatio; // Adjust width to maintain aspect ratio
    }
    else
    {
        imageWidth = windowSize.x; // Take up the full window width
        imageHeight = imageWidth / aspectRatio; // Adjust height to maintain aspect ratio
    }

    float imageX = (windowSize.x - imageWidth) / 2.0f;
    float imageY = (windowSize.y - imageHeight) / 2.0f;

    // Display the image
    ImGui::SetCursorPos(ImVec2(imageX, imageY));
    ImGui::Image(myGfx->myCameraShaderResourceView.Get(), ImVec2(imageWidth, imageHeight));

    ImGui::End();
}

void ImGuiRenderer::RenderFileHierarchyWindow(ImGuiID dockspace_id)
{
    ImGui::SetNextWindowDockID(dockspace_id, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(200, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin("File Hierarchy");

	RenderFileHierarchy("../Bin/Assets/");

    ImGui::End();
}

void ImGuiRenderer::RenderComponentsWindow(ImGuiID dockspace_id)
{
	ImGui::Begin("Components");
	ImGui::SetNextWindowDockID(dockspace_id, ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_FirstUseEver);
	// Render windows for each GameObject
	if (myGfx->mySelectedGameObject != nullptr)
	{
		for (Component* component : myGfx->mySelectedGameObject->myComponents)
		{
			component->RenderImGui();
			ImGui::Separator();
		}

		// Button for adding components
		if (ImGui::Button("Add Component"))
		{
			ImGui::OpenPopup("AddComponentPopup");
		}

		// Popup for selecting a script to add as a component
		if (ImGui::BeginPopup("AddComponentPopup"))
		{
			// Display a list of available scripts for components
			const std::filesystem::path componentPath = "../Source/Entity Component System/Components";
			std::set<std::string> uniqueFilenames;  // Store filenames

			for (const auto& entry : std::filesystem::directory_iterator(componentPath))
			{
				if (entry.is_regular_file())
				{
					std::string filename = entry.path().filename().stem().string();

					// Check if the filename is already added
					if (uniqueFilenames.find(filename) == uniqueFilenames.end())
					{
						uniqueFilenames.insert(filename);

						if (ImGui::Selectable(filename.c_str()))
						{
							// For example, using a factory pattern:
							Component* newComponent = ComponentFactory::CreateComponent(filename);

							if (newComponent)
							{
								// Add the new component to the GameObject
								newComponent->myGameObject = myGfx->mySelectedGameObject;
								newComponent->Init();
								myGfx->mySelectedGameObject->AddComponent(newComponent);
							}
						}
					}
				}
			}

			ImGui::EndPopup();
		}
	}
	ImGui::End();
}

void ImGuiRenderer::RenderSceneWindow()
{
	ImGui::Begin("Scene");
	for (GameObject* gameObject : myGfx->myGameObjects)
	{
		RenderGameObjectHierarchy(gameObject);
	}

	if (ImGui::BeginPopupContextWindow())
	{
		if (ImGui::MenuItem("Create GameObject"))
		{
			// Create a new GameObject and add it to myGameObjects
			GameObject* newGameObject = new GameObject();
			newGameObject->myTransform = dynamic_cast<TransformComponent*>(ComponentFactory::CreateComponent(ComponentFactory::TypeToString(Component::eComponentType::Transform)));
			newGameObject->AddComponent(newGameObject->myTransform);
			newGameObject->myTransform->Init();
			newGameObject->Init(myGfx->myTimer, &myGfx->myCamera, myGfx->myDevice, myGfx->myDeviceContext);
			myGfx->myGameObjects.push_back(newGameObject);

		}
		ImGui::EndPopup();
	}

	ImGui::End();
}


void ImGuiRenderer::RenderGameObjectHierarchy(GameObject* gameObject)
{
	if (ImGui::TreeNode(gameObject->myName.c_str()))
	{
		if (ImGui::IsItemClicked())
		{
			// Set the selectedGameObject variable to the clicked GameObject
			myGfx->mySelectedGameObject = gameObject;
		}

		for (GameObject* child : gameObject->GetChildren())
		{
			RenderGameObjectHierarchy(child);
		}

		ImGui::TreePop();
	}
}

void ImGuiRenderer::RenderFileHierarchy(const std::filesystem::path& aDirectory)
{
	std::exception_ptr eptr;
	try
	{
		for (const auto& entry : std::filesystem::directory_iterator(aDirectory))
		{
			const auto& path = entry.path();
			const auto& filename = path.filename().string();

			if (std::filesystem::is_directory(path))
			{
				if (ImGui::TreeNode(filename.c_str()))
				{
					RenderFileHierarchy(path);
					ImGui::TreePop();
				}
			}
			else
			{
				ImGui::Selectable(filename.c_str());
			}
		}
	}
	catch (...)
	{
		eptr = std::current_exception();
	}

	if (eptr)
	{
		try
		{
			std::rethrow_exception(eptr);
		}
		catch (const std::exception& e)
		{
			std::cout << "Caught exception \"" << e.what() << "\"\n";
		}
	}
}
