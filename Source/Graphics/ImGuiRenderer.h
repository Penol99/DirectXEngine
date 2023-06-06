#pragma once
#include "../ImGui/imgui.h"
#include <string>

namespace std
{
    namespace filesystem
    {
        class path;
    }
}
class Graphics;
class GameObject;
class ImGuiRenderer
{
public:
    void Init(Graphics* aGfx);
    void Render(int aWidth, int aHeight);

    void RenderMainMenu();

    std::string OpenFileSaveDialog();

    std::string OpenFileLoadDialog();

private:
    void RenderCameraWindow(ImGuiID dockspace_id);
    void RenderFileHierarchyWindow(ImGuiID dockspace_id);
    void RenderComponentsWindow(ImGuiID dockspace_id);
    void RenderSceneWindow();
    void RenderGameObjectHierarchy(GameObject* gameObject);
    void RenderFileHierarchy(const std::filesystem::path& aDirectory);
    int myWidth;
    int myHeight;
    Graphics* myGfx;
};

