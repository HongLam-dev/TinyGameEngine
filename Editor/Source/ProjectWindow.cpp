#include <imgui.h>
#include <imgui-SFML.h>
#include "ProjectWindow.h"

namespace TinyEditor {
	void ProjectWindow::Draw() {
        BeginWindow("Project");
        CheckActive();
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
                selectedFile = entry;
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
                        loadSceneCallback(entry.path().string());
                    }
                }
            }
        }
        EndWindow();
	}

    void ProjectWindow::DuplicateSelectedObject() {
        if (selectedFile.is_regular_file())
        {
            int n = 1;
            auto source = selectedFile.path();
            auto destination =source.parent_path() /
                (source.stem().string() + std::to_string(n) + source.extension().string());
            while (std::filesystem::exists(destination))
            {
                n++;
                destination = source.parent_path() /
                    (source.stem().string() + std::to_string(n) + source.extension().string());
            }

            std::filesystem::copy_file(source, destination);
        }
    }
}