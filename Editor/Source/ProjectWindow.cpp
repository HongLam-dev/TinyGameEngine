#include <imgui.h>
#include <imgui-SFML.h>
#include "ProjectWindow.h"

namespace TinyEditor {
	void ProjectWindow::Draw() {
        BeginWindow("Project");
        if (ImGui::Button("< Back"))
        {
            if(currentDirectory!=projectDirectory)
                currentDirectory = currentDirectory.parent_path();
        }
        for (const auto& entry :
            std::filesystem::directory_iterator(currentDirectory))
        {
            std::string name = entry.path().filename().string();

            if (isRenaming && entry == renamingFile)
            {
                ImGui::SetKeyboardFocusHere();

                if (ImGui::InputText(
                    "##Rename",
                    renameBuffer,
                    sizeof(renameBuffer),
                    ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    auto newPath =
                        entry.path().parent_path() /
                        (std::string(renameBuffer) +
                            entry.path().extension().string());

                    if (!std::filesystem::exists(newPath))
                    {
                        std::filesystem::rename(
                            entry.path(),
                            newPath
                        );
                    }

                    isRenaming = false;
                }

                if (ImGui::IsItemDeactivated())
                {
                    isRenaming = false;
                }
            }
            else
            {
                if (ImGui::Selectable(
                    name.c_str(),
                    entry.path() == selectedFile))
                {
                    selectedFile = entry;
                }

                if (ImGui::BeginPopupContextItem())
                {
                    if (ImGui::MenuItem("Rename"))
                    {
                        renamingFile = entry;

                        std::string stem =
                            entry.path().stem().string();

                        std::strncpy(
                            renameBuffer,
                            stem.c_str(),
                            sizeof(renameBuffer) - 1
                        );

                        renameBuffer[
                            sizeof(renameBuffer) - 1
                        ] = '\0';

                        isRenaming = true;
                    }

                    ImGui::EndPopup();
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
                            loadSceneCallback(
                                entry.path().string()
                            );
                        }
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