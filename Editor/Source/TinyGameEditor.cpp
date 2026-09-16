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

using namespace TinyEngine;

namespace TinyEditor {

    void TinyGameEditor::Run() {
        sf::RenderWindow* renderWindow = window.GetRenderWindow();
        TinyEditor::InputHandler inputHandler;

        editingScene = std::make_unique<Scene>();
        GameObject& mainCameraObj = editingScene->CreateMainCamera(engine);
        engine.ActivateScene(*editingScene);

        GameObject editorCameraObj(engine);
        Camera& editorCamera = editorCameraObj.AddComponent<Camera>();

        sf::Texture* placeHolderTex = textureManager.GetTexture("Assets/heart.png");

        GameObject& anchor = CreateASimpleBox(*editingScene, engine, {}, { 64,64,64 }, placeHolderTex);
        anchor.SetName("Anchor");

        GameObject* selectedObject = &mainCameraObj;

        ImGui::SFML::Init(*renderWindow);

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
            inputHandler.HandleSceneInput(editorCameraObj, 5, deltaTime.asSeconds());
            renderWindow->clear();

            engine.Render(window, editorCamera);
            DrawHierachyWindow(selectedObject);

            DrawInspectorWindow(selectedObject);
            ImGui::SFML::Render(*renderWindow);

            renderWindow->display();
            Input::Get().EndFrame();
        }

        ImGui::SFML::Shutdown();
    }

    void TinyGameEditor::DrawHierachyWindow( GameObject*& selectedObject)
    {
        ImGui::SetNextWindowSize(ImVec2(300, 1280), ImGuiCond_FirstUseEver);

        ImGui::Begin("Scene (1234567890)");


        if (ImGui::BeginPopupContextWindow())
        {
            if (ImGui::MenuItem("Create Empty"))
            {
                editingScene->CreateSceneObject(engine);
            }

            if (ImGui::MenuItem("Create Sprite"))
            {
                // Create GameObject + SpriteRenderer
            }

            ImGui::EndPopup();
        }

        for (auto& object : editingScene->GetGameObjects())
        {
            if (ImGui::Selectable(
                object->GetName().c_str(),
                selectedObject == object.get()))
            {
                selectedObject = object.get();
            }
        }

        ImGui::End();
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

        if (selectedObject)
        {
            ImGui::Text("%s", selectedObject->GetName().c_str());
            DrawTransform(*selectedObject);
        }

        ImGui::End();
    }

    GameObject& TinyGameEditor::CreateASimpleBox(Scene& scene,
        TinyEngine::TinyGameEngine& engine,
        const Vector3& position,
        const Vector3& size,
        sf::Texture* boxTexture) {
        GameObject& objectRef = scene.CreateSceneObject(engine);

        if (boxTexture)
        {
            SpriteRenderer& renderer =
                objectRef.AddComponent<SpriteRenderer>();
            renderer.SetTexture(*boxTexture);
        }
        BoxCollider2D& collider =
            objectRef.AddComponent<BoxCollider2D>();

        collider.SetSize({
            PixelsToWorld(size.x),
            PixelsToWorld(size.y),
            PixelsToWorld(size.z)
            });

        objectRef.GetComponent<Transform>()
            ->SetPosition({
                PixelsToWorld(position.x),
                PixelsToWorld(position.y),
                position.z
                });
        return objectRef;
    }


}