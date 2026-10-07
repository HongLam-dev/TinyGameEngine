#include "Hierarchy.h"
#include <imgui.h>
#include <imgui-SFML.h>
using namespace TinyEngine;

namespace TinyEditor {
    void Hierarchy::Draw(Scene& scene, GameObject*& selectedObject, TinyEngine::Camera& editorCamera)
    {
        ImGui::SetNextWindowSize(ImVec2(300, 1280), ImGuiCond_FirstUseEver);

        BeginWindow(scene.GetName()+"                                                       1234567890qwertyuiopasdfghjklzxcvbnmQWERTYUIOPSDFGHJKLZXMNCBV';:/\"\\ (Don't ask why)");
        CheckActive();
        if (ImGui::BeginPopupContextWindow())
        {
            if (ImGui::MenuItem("Create Empty"))
            {
                scene.CreateSceneObject();
            }

            if (ImGui::MenuItem("Create Rectangle"))
            {
                scene.CreateASimpleBox({}, { 0.64f,0.64f,0 });
            }

            if (ImGui::MenuItem("UI Object"))
            {
                scene.CreateUIObject();
            }
            ImGui::EndPopup();
        }

        for (auto& object : scene.GetGameObjects())
        {
            DrawGameObject(*object.get(), selectedObject,editorCamera,scene);
        }

        EndWindow();
    }

    void Hierarchy::DrawGameObject(
        TinyEngine::GameObject& object,
        TinyEngine::GameObject*& selectedObject,
        TinyEngine::Camera& editorCamera,
        Scene& scene
    ){
        ImGui::PushID(&object);

        if (renamingObject == &object)
        {
            ImGui::SetKeyboardFocusHere();
            if (ImGui::InputText(
                "##Rename",
                renameBuffer,
                sizeof(renameBuffer),
                ImGuiInputTextFlags_EnterReturnsTrue))
            {
                object.SetName(renameBuffer);
                renamingObject = nullptr;
            }
            if (ImGui::IsItemDeactivated())
            {
                object.SetName(renameBuffer);
                renamingObject = nullptr;
            }
        }
        else
        {
            if (ImGui::Selectable(
                object.GetName().c_str(),
                selectedObject == &object))
            {
                selectedObject = &object;           
            }
            if (ImGui::BeginDragDropSource())
            {
                TinyEngine::GameObject* objectPtr = &object;

                ImGui::SetDragDropPayload(
                    "GAMEOBJECT",
                    &objectPtr,
                    sizeof(objectPtr)
                );

                ImGui::Text("%s", object.GetName().c_str());

                ImGui::EndDragDropSource();
            }
            if (ImGui::IsItemHovered() &&
                ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            {
                editorCamera.SetPosition(
                    object.GetTransform().GetPosition()
                );
            }

            if (ImGui::BeginPopupContextItem())
            {
                if (ImGui::MenuItem("Rename"))
                {
                    selectedObject = &object;
                    RenameObject(object);
                }

                if (ImGui::MenuItem("Delete"))
                {
                    selectedObject = nullptr;
                    scene.DestroySceneObject(object);
                }

                ImGui::EndPopup();
            }
        }

        ImGui::PopID();
    }

    void Hierarchy::RenameObject(
        TinyEngine::GameObject& object
    ) {
        renamingObject = &object;

        std::strncpy(
            renameBuffer,
            object.GetName().c_str(),
            sizeof(renameBuffer) - 1
        );

        renameBuffer[sizeof(renameBuffer) - 1] = '\0';
    }
}