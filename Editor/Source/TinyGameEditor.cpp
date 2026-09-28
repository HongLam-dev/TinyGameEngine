#include "TinyGameEditor.h"
#include "TinyGameEngine.h"
#include "Window.h"
#include "Scene.h"
#include "InputHandler.h"
#include "EngineSettings.h"
#include "GameComponentRegister.h"
#include "EngineComponentRegister.h"
#include "SceneSerializer.h"
#include "Hierarchy.h"
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
        TGModule::GameComponentRegister::RegisterGameComponents();
        EngineComponentRegister::RegisterEngineComponents();
        EngineComponentRegister::RegisterEngineEnums();
    }

    void TinyGameEditor::Run() {
        sf::RenderWindow* renderWindow = window.GetRenderWindow();
        TinyEditor::InputHandler inputHandler(window, [this]() { SaveScene();}, [this](std::string sceneName) { LoadScene(sceneName); });
        if (!LoadScene("Example Scene"))
        {
            CreateNewScene();
        }

        GameObject editorCameraObj(engine);
        editorCamera = &editorCameraObj.AddComponent<Camera>();

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
            renderWindow->clear(sceneColor);

            workingWindow = WorkingWindow::Scene;
            engine.Render(window, *editorCamera);
            DrawObjectMarker(selectedObject);

            hierarchy.Draw(*editingScene,selectedObject,*editorCamera);
            inspector.Draw(selectedObject);

            ImGui::SFML::Render(*renderWindow);

            renderWindow->display();

            if (workingWindow == WorkingWindow::Scene)
                inputHandler.HandleSceneInput(*editorCamera, selectedObject, deltaTime.asSeconds());
            inputHandler.HandleGlobalInput();
            Input::Get().EndFrame();
        }

        ImGui::SFML::Shutdown();
    }

    void TinyGameEditor::DrawObjectMarker(TinyEngine::GameObject*& selectedObject) {
        if (selectedObject)
        {
            Vector3 position = selectedObject->GetTransform().GetPosition();
            Vector2 screenPos = editorCamera->WorldToScreenPosition(position, window.GetSize());
            DrawMarker({ screenPos.x,screenPos.y });
        }
    }
    void TinyGameEditor::DrawMarker(sf::Vector2f pixelPosition)
    {
        sf::Texture* tex= TextureManager::Instance().GetTexture("Assets/Hand.png");
        if (tex)
        {
            sf::Sprite marker(
                *tex
            );

            sf::Vector2u size = marker.getTexture().getSize();

            marker.setOrigin(
                sf::Vector2f(
                    (size.x / 2.0f),
                    (size.y / 2.0f)
                )
            );

            marker.setPosition(pixelPosition);

            window.GetRenderWindow()->draw(marker);
        }
        else
        {
            std::cout << "No texture for marker found\n";
        }

    }

   void TinyGameEditor::SaveScene() {
       if (!editingScene)
           return;
       TGModule::SceneSerializer::Instance().SaveScene(*editingScene);

       std::cout << "Scene Saved\n";
   }

   bool TinyGameEditor::LoadScene(std::string sceneName) {
       auto emptyScene = std::make_unique<Scene>(engine);

       if (TGModule::SceneSerializer::Instance().LoadScene(sceneName, *emptyScene))
       {
           editingScene = std::move(emptyScene);
           selectedObject = nullptr;
           engine.ActivateScene(*editingScene);
           return true;
       }
       else {
           return false;
       }

   }

   void TinyGameEditor::CreateNewScene()
   {
       editingScene= std::make_unique<Scene>(engine);
       editingScene->CreateMainCamera();
   }
}