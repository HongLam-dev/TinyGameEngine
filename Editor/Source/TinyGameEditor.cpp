#include "TinyGameEditor.h"
#include "TinyGameEngine.h"
#include "Window.h"
#include "Scene.h"
#include "InputHandler.h"
#include "EngineSettings.h"
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <iostream>
#include <TextureManager.h>
#include <memory>
#include <cstring>

using namespace TinyEngine;

namespace TinyEditor {

    void TinyGameEditor::Run() {
        sf::RenderWindow* renderWindow = window.GetRenderWindow();
        TinyEditor::InputHandler inputHandler;

        editingScene = std::make_unique<Scene>(engine);
        GameObject& mainCameraObj = editingScene->CreateMainCamera();
        engine.ActivateScene(*editingScene);

        Camera& editorCamera = editorCameraObj.AddComponent<Camera>();

        sf::Texture* placeHolderTex = textureManager.GetTexture("Assets/heart.png");

        GameObject& anchor = editingScene->CreateASimpleBox({-3.0f,3.0f,0}, { 0.64f,0.64f,0 }, placeHolderTex);
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
                inputHandler.HandleSceneInput(editorCameraObj, 5, deltaTime.asSeconds());

            renderWindow->clear(sceneColor);

            workingWindow = WorkingWindow::Scene;
            engine.Render(window, editorCamera);
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
            Vector3 pixelPos;
           //     DrawMarker(WorldToPixels(position));
        }
    }
    void TinyGameEditor::DrawMarker(sf::Vector2f  pixelPosition) {
        sf::VertexArray marker(sf::PrimitiveType::Lines, 4);

        float size = 10.0f;

        marker[0].position = pixelPosition + sf::Vector2f(-size, 0);
        marker[1].position = pixelPosition + sf::Vector2f(size, 0);

        marker[2].position = pixelPosition + sf::Vector2f(0, -size);
        marker[3].position = pixelPosition + sf::Vector2f(0, size);

        marker[0].color = sf::Color::Yellow;
        marker[1].color = sf::Color::Yellow;
        marker[2].color = sf::Color::Yellow;
        marker[3].color = sf::Color::Yellow;

        window.GetRenderWindow()->draw(marker);
    }

    void TinyGameEditor::DrawTransform(GameObject& gameObject) {
        ImGui::Text("%s", "Transform------");

        Transform& transform = gameObject.GetTransform();
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
        ImGui::SetNextWindowSize(ImVec2(300, 1280), ImGuiCond_FirstUseEver);

        ImGui::Begin("Inspector");


        if (ImGui::IsWindowHovered())
            workingWindow = WorkingWindow::Other;
        if (selectedObject)
        {
            ImGui::Text("%s", selectedObject->GetName().c_str());
            DrawTransform(*selectedObject);
        }

        ImGui::End();
    }
}