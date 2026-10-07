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
#include "Helpers.h"
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <iostream>
#include <TextureManager.h>
#include <memory>
#include <cstring>


using namespace TinyEngine;

namespace TinyEditor {
    TinyGameEditor::TinyGameEditor() :engine(window){
        projectWindow.SetProjectDirectory(projectDirectory);
        RegisterComponents();
    }

    void TinyGameEditor::RegisterComponents() {
        TGModule::GameComponentRegister::RegisterGameComponents();
        EngineComponentRegister::RegisterEngineComponents();
        EngineComponentRegister::RegisterEngineEnums();
    }

    void TinyGameEditor::Run() {
        sf::RenderWindow* renderWindow = window.GetRenderWindow();
        TinyEditor::InputHandler inputHandler(window, [this]() { SaveScene();} );

        projectWindow.SetLoadSceneCallback([this](std::string sceneName) {
            LoadScene(sceneName);
            });
        inputHandler.SetDuplicateCallback([this]() {DuplicateObject(); });

     /*   if (!LoadScene(projectDirectory+'\\'+defaultSceneFolder + "\\Example Scene.tge"))
        {
            CreateNewScene();
        }*/
      if (!LoadScene("Scenes\\Example Scene.tge"))
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
            sf::Time elapsedTime = deltaClock.restart();

            if(!isGameRuning)
            {
                ImGui::SFML::Update(*renderWindow, elapsedTime);
                renderWindow->clear(sceneColor);

                scenePreview.Draw(window, engine, editingScene.get(), *editorCamera, selectedObject);
                hierarchy.Draw(*editingScene, selectedObject, *editorCamera);
                inspector.Draw(selectedObject);
                projectWindow.Draw();
                DrawGadgetBar();

                ImGui::SFML::Render(*renderWindow);
                renderWindow->display();

                if (!inspector.IsActiveWindow() && !hierarchy.IsActiveWindow() && !projectWindow.IsActiveWindow())
                {
                    inputHandler.HandleSceneInput(*editorCamera, selectedObject, elapsedTime.asSeconds());
                }
                inputHandler.HandleGlobalInput();
                Input::Get().EndFrame();
            }
            else {
                static float accumulatedTimeStep = 0;
                static float deltaTime = 0;
                deltaTime += elapsedTime.asSeconds();
                accumulatedTimeStep += elapsedTime.asSeconds();

                if (deltaTime >= 1.0 / EngineSettings::targetFPS)
                {
                    engine.Update(deltaTime);
                }

                while (accumulatedTimeStep >= 1.0 /EngineSettings::timeStep)
                {
                    engine.FixedUpdate(1.0 / EngineSettings::timeStep);

                    accumulatedTimeStep -= 1.0 / EngineSettings::timeStep;
                }

                if (deltaTime >= 1.0 / EngineSettings::targetFPS)
                {
                    ImGui::SFML::Update(*renderWindow, elapsedTime);
                    renderWindow->clear(sceneColor);

                    engine.Render(window, *editingScene->GetMainCamera());
                    hierarchy.Draw(*editingScene, selectedObject, *editorCamera);
                    inspector.Draw(selectedObject);
                    projectWindow.Draw();
                    DrawGadgetBar();

                    ImGui::SFML::Render(*renderWindow);
                    renderWindow->display();

                    engine.HandleReferredActions();
                    deltaTime = 0;
                    Input::Get().EndFrame();
                }
            }
   
        }

        ImGui::SFML::Shutdown();
    }

    void TinyGameEditor::DrawGadgetBar() {
        ImGui::Begin("Gadgets");
        if (ImGui::Button("Run Game"))
        {
            isGameRuning = !isGameRuning;
            if (isGameRuning)
            {
                engine.ActivateScene(*editingScene);
            }
        }
        ImGui::End();
    }


    void TinyGameEditor::SaveScene()
    {
        if (!editingScene)
            return;
        std::string sceneFileName = editingScene->GetName() + sceneExtension;

        std::string path = SelectSaveFile(projectDirectory + '\\' + defaultSceneFolder + '\\'+sceneFileName);
        if (path.empty())
            return;

        TGModule::SceneSerializer::Instance()
            .SaveScene(*editingScene, path);

        std::cout << "Scene saved at " << path;;
    }

   bool TinyGameEditor::LoadScene(std::string scenePath) {
       if (TGModule::SceneSerializer::Instance().FindScene(scenePath))
       {
           editingScene = std::make_unique<Scene>(engine);
           TGModule::SceneSerializer::Instance().LoadScene(scenePath, *editingScene);
           selectedObject = nullptr;
           engine.ActivateScene(*editingScene);
           return true;
       }
       else {
           return false;
       }

   }
   void TinyGameEditor::DuplicateObject() {
       if (projectWindow.IsActiveWindow())
       {
           projectWindow.DuplicateSelectedObject();
       }
       else if (hierarchy.IsActiveWindow())
       {
           if(selectedObject)
                editingScene->DuplicateObject(*selectedObject);
       }
   }

   void TinyGameEditor::CreateNewScene()
   {
       editingScene= std::make_unique<Scene>(engine);
       editingScene->CreateMainCamera();
   }
}