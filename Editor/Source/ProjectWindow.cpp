#include <imgui.h>
#include <imgui-SFML.h>
#include "ProjectWindow.h"

namespace TinyEditor {
	void ProjectWindow::Draw() {
        ImGui::Begin("Project");
        if (ImGui::Button("< Back"))
        {
            if(currentDirectory!=projectDirectory)
                currentDirectory = currentDirectory.parent_path();
        }
        for (const auto& entry :
            std::filesystem::directory_iterator(currentDirectory))
        {
            std::string name = entry.path().filename().string();

            if (ImGui::Selectable(name.c_str(), entry.path() ==selectedFile ))
            {
                selectedFile = entry.path();     
            }
            if (ImGui::IsItemHovered() &&
                ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            {
                if (entry.is_directory())
                {
                    currentDirectory = entry.path();
                }
                else if (entry.is_regular_file())
                {
                    if (entry.path().extension() == ".tge")
                    {
                        loadSceneCallback(entry.path().filename().string());
                    }
                }
            }
        }
        ImGui::End();
	}
}