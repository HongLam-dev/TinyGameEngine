#include "TinyGameEditor.h"
#include "TinyGameEngine.h"
#include "Window.h"
#include "Scene.h"
#include "InputHandler.h"
#include "EngineSettings.h"
#include "GameComponentRegister.h"
#include "EngineComponentRegister.h"
#include "SceneSerializer.h"
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
            DrawHierachyWindow(selectedObject, renamingObject);

            DrawInspectorWindow(selectedObject);
            ImGui::SFML::Render(*renderWindow);

            renderWindow->display();

            if (workingWindow == WorkingWindow::Scene)
                inputHandler.HandleSceneInput(*editorCamera, selectedObject, deltaTime.asSeconds());
            inputHandler.HandleGlobalInput();
            Input::Get().EndFrame();
        }

        ImGui::SFML::Shutdown();
    }

    void TinyGameEditor::DrawHierachyWindow(GameObject*& selectedObject, GameObject*& renamingObject)
    {
        ImGui::SetNextWindowSize(ImVec2(300, 1280), ImGuiCond_FirstUseEver);

        ImGui::Begin("Scene                                                       1234567890qwertyuiopasdfghjklzxcvbnmQWERTYUIOPSDFGHJKLZXMNCBV';:/\"\\ (Don't ask why)");

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
                editingScene->CreateASimpleBox({}, { 0.64f,0.64f,0 });
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
                if (ImGui::IsItemHovered() &&
                    ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                {
                    editorCamera->SetPosition(
                        object->GetTransform().GetPosition()
                    );
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

                    if (ImGui::MenuItem("Delete"))
                    {
                        selectedObject = nullptr;
                        editingScene->DestroySceneObject(*object.get());
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
           DrawSelectedObject(*selectedObject);
           DrawAddComponentMenu(*selectedObject);
       }

       ImGui::End();
   }


   void TinyGameEditor::DrawSelectedObject(GameObject& gameObject)
   {
       ImGui::Text("%s", gameObject.GetName().c_str());
       for (const auto& component : gameObject.GetAllComponents())
       {
           DrawComponent(*component);
       }
   }


   void TinyGameEditor::DrawComponent(Component& component)
   {
       std::type_index type = typeid(component);

       const ComponentInfo* info =
           componentRegister.FindComponent(type);

       if (!info)
           return;

       ImGui::PushID(&component);

       bool open = ImGui::CollapsingHeader(
           info->name.c_str(),
           ImGuiTreeNodeFlags_DefaultOpen
       );


       if (open)
       {
           for (const auto& field : info->fields)
           {
               DrawField(component, field);
           }
           if (ImGui::SmallButton("X"))
           {
               selectedObject->Destroy(component);
           }
       }

       ImGui::PopID();
   }

   void TinyGameEditor::DrawField(
       Component& component,
       const FieldInfo& field)
   {
       std::any value = field.getValue(component);
       bool isFieldInteracted=false;
       switch (field.type)
       {
       case FieldType::Float:
           isFieldInteracted = DrawFloatField(component, field, value);
           break;

       case FieldType::Vector2:
           isFieldInteracted = DrawVector2Field(component, field, value);
           break;

       case FieldType::Vector3:
           isFieldInteracted = DrawVector3Field(component, field, value);
           break;

       case FieldType::Int:
           isFieldInteracted = DrawIntField(component, field, value);
           break;

       case FieldType::Bool:
           isFieldInteracted = DrawBoolField(component, field, value);
           break;

       case FieldType::String:
           isFieldInteracted = DrawStringField(component, field, value);
           break;

       case FieldType::IntRect:
           isFieldInteracted = DrawIntRectField(component, field, value);
           break;
       }
       if (isFieldInteracted)
       {
           workingWindow = WorkingWindow::Other;
       }
   }


   bool TinyGameEditor::DrawFloatField(
       Component& component,
       const FieldInfo& field,
       const std::any& value)
   {
       float valueFloat =
           std::any_cast<float>(value);

       if (ImGui::DragFloat(
           field.name.c_str(),
           &valueFloat))
       {
           field.setValue(component, valueFloat);
           return true;
       }
       return false;
   }


   bool TinyGameEditor::DrawVector2Field(
       Component& component,
       const FieldInfo& field,
       const std::any& value)
   {
       Vector2 valueVector =
           std::any_cast<Vector2>(value);

       if (ImGui::DragFloat2(
           field.name.c_str(),
           &valueVector.x))
       {
           field.setValue(component, valueVector);
           return true;
       }
       return false;
   }


   bool TinyGameEditor::DrawVector3Field(
       Component& component,
       const FieldInfo& field,
       const std::any& value)
   {
       Vector3 valueVector =
           std::any_cast<Vector3>(value);

       if (ImGui::DragFloat3(
           field.name.c_str(),
           &valueVector.x))
       {
           field.setValue(component, valueVector);
           return true;
       }
       return false;
   }


   bool TinyGameEditor::DrawIntField(
       Component& component,
       const FieldInfo& field,
       const std::any& value)
   {
       int valueInt =
           std::any_cast<int>(value);

       if (ImGui::DragInt(
           field.name.c_str(),
           &valueInt))
       {
           field.setValue(component, valueInt);
           return true;
       }
       return false;
   }


   bool TinyGameEditor::DrawBoolField(
       Component& component,
       const FieldInfo& field,
       const std::any& value)
   {
       bool valueBool =
           std::any_cast<bool>(value);

       if (ImGui::Checkbox(
           field.name.c_str(),
           &valueBool))
       {
           field.setValue(component, valueBool);
           return true;
       }
       return false;
   }


   bool TinyGameEditor::DrawStringField(
       Component& component,
       const FieldInfo& field,
       const std::any& value)
   {
       std::string valueString =
           std::any_cast<std::string>(value);

       if (editingField != &field ||
           editingComponent != &component)
       {
           std::strncpy(
               fieldStringBuffer,
               valueString.c_str(),
               sizeof(fieldStringBuffer) - 1
           );

           fieldStringBuffer[
               sizeof(fieldStringBuffer) - 1
           ] = '\0';

           editingField = &field;
           editingComponent = &component;
       }

       if (ImGui::InputText(
           field.name.c_str(),
           fieldStringBuffer,
           sizeof(fieldStringBuffer),
           ImGuiInputTextFlags_EnterReturnsTrue))
       {
           field.setValue(
               component,
               std::string(fieldStringBuffer)
           );

           editingField = nullptr;
           editingComponent = nullptr;
       }

       if (ImGui::IsItemDeactivated())
       {
           field.setValue(
               component,
               std::string(fieldStringBuffer)
           );

           editingField = nullptr;
           editingComponent = nullptr;

       }
       return false;
   }


   bool TinyGameEditor::DrawIntRectField(
       Component& component,
       const FieldInfo& field,
       const std::any& value)
   {
       sf::IntRect rect =
           std::any_cast<sf::IntRect>(value);

       int values[4] =
       {
           rect.position.x,
           rect.position.y,
           rect.size.x,
           rect.size.y
       };

       if (ImGui::DragInt4(
           field.name.c_str(),
           values))
       {
           rect.position.x = values[0];
           rect.position.y = values[1];
           rect.size.x = values[2];
           rect.size.y = values[3];

           field.setValue(component, rect);
           return true;
       }
       return false;
   }


   void TinyGameEditor::DrawAddComponentMenu(
       GameObject& gameObject)
   {
       if (ImGui::Button("Add Component"))
       {
           ImGui::OpenPopup("AddComponent");
       }

       if (!ImGui::BeginPopup("AddComponent"))
           return;

       for (const auto& [type, info] :
           componentRegister.GetComponentList())
       {
           if (ImGui::MenuItem(info.name.c_str()))
           {
               info.create(gameObject);
           }
       }

       ImGui::EndPopup();
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