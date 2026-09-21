#include "TinyGameEditor.h"
#include "TinyGameEngine.h"
#include "Window.h"
#include "Scene.h"
#include "InputHandler.h"
#include "EngineSettings.h"
#include "GameComponentRegister.h"
#include "EngineComponentRegister.h"
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <iostream>
#include <TextureManager.h>
#include <memory>
#include <cstring>

using namespace TinyEngine;

namespace TinyEditor {
    TinyGameEditor::TinyGameEditor() :engine(window) {
        RegisterComponents();
    }

    void TinyGameEditor::RegisterComponents() {
        TinyGame::GameComponentRegister::RegisterGameComponents(componentRegister);
        EngineComponentRegister::RegisterEngineComponents(componentRegister);
    }

    void TinyGameEditor::Run() {
        sf::RenderWindow* renderWindow = window.GetRenderWindow();
        TinyEditor::InputHandler inputHandler(window);

        editingScene = std::make_unique<Scene>(engine);
        GameObject& mainCameraObj = editingScene->CreateMainCamera();
        engine.ActivateScene(*editingScene);

        GameObject editorCameraObj(engine);
        editorCamera = &editorCameraObj.AddComponent<Camera>();

        sf::Texture* placeHolderTex = textureManager.GetTexture("Assets/heart.png");

        GameObject& anchor = editingScene->CreateASimpleBox({}, { 0.64f,0.64f,0 }, placeHolderTex);
        anchor.SetName("Anchor");


        GameObject* renamingObject = nullptr;

        if (!ImGui::SFML::Init(*renderWindow))
        {
            std::cout << "Failed to initialize ImGui window";
            return;
        }

        ImGui::GetIO().Fonts->AddFontDefault();


        sf::Clock deltaClock;
        while (renderWindow->isOpen())
        {
            while (const std::optional event = renderWindow->pollEvent())
            {
                ImGui::SFML::ProcessEvent(*renderWindow, *event);
                Input::Get().ProcessEvent(*event);

                if (event->is<sf::Event::Closed>())
                    renderWindow->close();
            }
            sf::Time deltaTime = deltaClock.restart();

            ImGui::SFML::Update(*renderWindow, deltaTime);

            if (workingWindow == WorkingWindow::Scene)
                inputHandler.HandleSceneInput(*editorCamera,selectedObject, deltaTime.asSeconds());

            renderWindow->clear(sceneColor);

            workingWindow = WorkingWindow::Scene;
            engine.Render(window, *editorCamera);
            DrawObjectMarker(selectedObject);
            DrawHierachyWindow(selectedObject, renamingObject);

            DrawInspectorWindow(selectedObject);
            ImGui::SFML::Render(*renderWindow);

            renderWindow->display();
            Input::Get().EndFrame();
        }

        ImGui::SFML::Shutdown();
    }

    void TinyGameEditor::DrawHierachyWindow( GameObject*& selectedObject, GameObject*& renamingObject)
    {
        ImGui::SetNextWindowSize(ImVec2(300, 1280), ImGuiCond_FirstUseEver);

        ImGui::Begin("Scene                                                       1234567890qwertyuiopasdfghjklzxcvbnmQWERTYUIOPSDFGHJKLZXMNCBV (Don't ask why)");

        if (ImGui::IsWindowHovered())
            workingWindow = WorkingWindow::Other;
        if (ImGui::BeginPopupContextWindow())
        {
            if (ImGui::MenuItem("Create Empty"))
            {
                editingScene->CreateSceneObject();
            }

            if (ImGui::MenuItem("Create Rectangle"))
            {
                editingScene->CreateASimpleBox({},{ 0.64f,0.64f,0});
            }

            ImGui::EndPopup();
        }

        for (auto& object : editingScene->GetGameObjects())
        {
            ImGui::PushID(object.get());

            if (renamingObject == object.get())
            {
                ImGui::SetKeyboardFocusHere();
                if (ImGui::InputText(
                    "##Rename",
                    renameBuffer,
                    sizeof(renameBuffer),
                    ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    object->SetName(renameBuffer);
                    renamingObject = nullptr;
                }
                if (ImGui::IsItemDeactivated())
                {
                    object->SetName(renameBuffer);
                    renamingObject = nullptr;
                }
            }
            else
            {
                if (ImGui::Selectable(
                    object->GetName().c_str(),
                    selectedObject == object.get()))
                {
                    selectedObject = object.get();
                }

                if (ImGui::BeginPopupContextItem())
                {
                    if (ImGui::MenuItem("Rename"))
                    {
                        selectedObject = object.get();
                        renamingObject = object.get();

                        std::strncpy(
                            renameBuffer,
                            object->GetName().c_str(),
                            sizeof(renameBuffer) - 1
                        );

                        renameBuffer[sizeof(renameBuffer) - 1] = '\0';
                    }

                    ImGui::EndPopup();
                }
            }

            ImGui::PopID();
        }

        ImGui::End();
    }


    void TinyGameEditor::DrawObjectMarker(TinyEngine::GameObject*& selectedObject) {
        if (selectedObject)
        {
            Vector3 position = selectedObject->GetTransform().GetPosition();
            Vector2 screenPos =editorCamera->WorldToScreenPosition(position,window.GetSize());
            DrawMarker({ screenPos.x,screenPos.y});
        }
    }
    void TinyGameEditor::DrawMarker(sf::Vector2f pixelPosition)
    {
        float size = 30.0f;
        float arrowSize = 8.0f;

        sf::VertexArray marker(sf::PrimitiveType::Lines, 12);

        // X axis
        marker[0].position = pixelPosition + sf::Vector2f(-size, 0);
        marker[1].position = pixelPosition + sf::Vector2f(size, 0);

        // X arrow head
        marker[2].position = pixelPosition + sf::Vector2f(size, 0);
        marker[3].position = pixelPosition + sf::Vector2f(size - arrowSize, -arrowSize / 2);

        marker[4].position = pixelPosition + sf::Vector2f(size, 0);
        marker[5].position = pixelPosition + sf::Vector2f(size - arrowSize, arrowSize / 2);

        // Y axis
        marker[6].position = pixelPosition + sf::Vector2f(0, -size);
        marker[7].position = pixelPosition + sf::Vector2f(0, size);

        // Y arrow head
        marker[8].position = pixelPosition + sf::Vector2f(0, -size);
        marker[9].position = pixelPosition + sf::Vector2f(-arrowSize / 2, -size + arrowSize);

        marker[10].position = pixelPosition + sf::Vector2f(0, -size);
        marker[11].position = pixelPosition + sf::Vector2f(arrowSize / 2, -size + arrowSize);

        // Colors
        marker[0].color = sf::Color::Red;
        marker[1].color = sf::Color::Red;
        marker[2].color = sf::Color::Red;
        marker[3].color = sf::Color::Red;
        marker[4].color = sf::Color::Red;
        marker[5].color = sf::Color::Red;

        marker[6].color = sf::Color::Yellow;
        marker[7].color = sf::Color::Yellow;
        marker[8].color = sf::Color::Yellow;
        marker[9].color = sf::Color::Yellow;
        marker[10].color = sf::Color::Yellow;
        marker[11].color = sf::Color::Yellow;

        window.GetRenderWindow()->draw(marker);
    }

    void TinyGameEditor::DrawTransform(TinyEngine::Transform& transform) {
        ImGui::Text("%s", "Transform------");

        Vector3 position = transform.GetPosition();
        Vector3 rotation = transform.GetRotation();
        Vector3 scale = transform.GetScale();

        ImGui::DragFloat3("Position", &position.x);
        ImGui::DragFloat3("Rotation", &rotation.x);
        ImGui::DragFloat3("Scale", &scale.x);

        transform.SetPosition(position);
        transform.SetRotation(rotation);
        transform.SetScale(scale);
    }

    void TinyGameEditor::DrawInspectorWindow(GameObject*& selectedObject)
    {
        ImGui::SetNextWindowSize(
            ImVec2(300, 1280),
            ImGuiCond_FirstUseEver
        );

        ImGui::Begin("Inspector");

        if (ImGui::IsWindowHovered())
            workingWindow = WorkingWindow::Other;

        if (selectedObject)
        {
            ImGui::Text("%s", selectedObject->GetName().c_str());
            // Draw existing components
            for (const auto& component : selectedObject->GetAllComponents())
            {
                if (typeid(*component)== typeid(Transform))
                {
                    Transform* transform = static_cast<Transform*>(component);
                    DrawTransform(*transform);
                }
                else {
                    std::type_index type = typeid(*component);

                    const ComponentInfo* info =
                        componentRegister.FindComponent(type);

                    if (!info)
                        continue;

                    if (ImGui::CollapsingHeader(
                        info->name.c_str(),
                        ImGuiTreeNodeFlags_DefaultOpen))
                    {
                        for (const auto& field : info->fields)
                        {
                            std::any value = field.getValue(*component);

                            switch (field.type)
                            {
                            case FieldType::Float:
                            {
                                float valueFloat = std::any_cast<float>(value);

                                if (ImGui::DragFloat(field.name.c_str(), &valueFloat))
                                {
                                    field.setValue(*component, valueFloat);
                                }

                                break;
                            }

                            case FieldType::Vector2:
                            {
                                Vector2 valueVector =
                                    std::any_cast<Vector2>(value);

                                if (ImGui::DragFloat2(
                                    field.name.c_str(),
                                    &valueVector.x))
                                {
                                    field.setValue(*component, valueVector);
                                }

                                break;
                            }

                            case FieldType::Vector3:
                            {
                                Vector3 valueVector =
                                    std::any_cast<Vector3>(value);

                                if (ImGui::DragFloat3(
                                    field.name.c_str(),
                                    &valueVector.x))
                                {
                                    field.setValue(*component, valueVector);
                                }

                                break;
                            }

                            case FieldType::Int:
                            {
                                int valueInt = std::any_cast<int>(value);

                                if (ImGui::DragInt(
                                    field.name.c_str(),
                                    &valueInt))
                                {
                                    field.setValue(*component, valueInt);
                                }

                                break;
                            }

                            case FieldType::Bool:
                            {
                                bool valueBool = std::any_cast<bool>(value);

                                if (ImGui::Checkbox(
                                    field.name.c_str(),
                                    &valueBool))
                                {
                                    field.setValue(*component, valueBool);
                                }

                                break;
                            }

                            case FieldType::String:
                                // Handle later
                                break;
                            }
                        }
                    }
                }
               
            }

            if (ImGui::Button("Add Component"))
            {
                ImGui::OpenPopup("AddComponent");
            }

            if (ImGui::BeginPopup("AddComponent"))
            {
                for (const auto& component : componentRegister.GetComponentList())
                {
                    if (ImGui::MenuItem(component.second.name.c_str()))
                    {
                        component.second.create(*selectedObject);
                    }
                }

                ImGui::EndPopup();
            }
        }

        ImGui::End();
    }
}