#pragma once
#include "TinyGameEngine.h"
#include "Window.h"
#include "Scene.h"
#include "InputHandler.h"
#include "EngineSettings.h"
#include "ComponentRegister.h"
#include "FieldInfo.h"
#include "TextureManager.h"
#include "Component.h"
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <iostream>
#include <functional>


namespace TinyEditor {
	class TinyGameEditor
	{
	public:
		enum class WorkingWindow
		{
			Scene,
			Other
		};
		TinyGameEditor();
		void Run();
		void DrawHierachyWindow(TinyEngine::GameObject*& selectedObject, TinyEngine::GameObject*& renamingObject);
		void DrawInspectorWindow(TinyEngine::GameObject*& selectedObject);
		void DrawMarker(sf::Vector2f  pixelPosition);
		void DrawObjectMarker(TinyEngine::GameObject*& selectedObject);
		void RegisterComponents();

        void SaveScene();
        void LoadScene();

        void DrawSelectedObject(TinyEngine::GameObject& gameObject);

        void DrawComponent(TinyEngine::Component& component);

        void DrawField(
            TinyEngine::Component& component,
            const TinyEngine::FieldInfo& field
        );

        bool DrawFloatField(
            TinyEngine::Component& component,
            const TinyEngine::FieldInfo& field,
            const std::any& value
        );

        bool DrawVector2Field(
            TinyEngine::Component& component,
            const TinyEngine::FieldInfo& field,
            const std::any& value
        );

        bool DrawVector3Field(
            TinyEngine::Component& component,
            const TinyEngine::FieldInfo& field,
            const std::any& value
        );

        bool DrawIntField(
            TinyEngine::Component& component,
            const TinyEngine::FieldInfo& field,
            const std::any& value
        );

        bool DrawBoolField(
            TinyEngine::Component& component,
            const TinyEngine::FieldInfo& field,
            const std::any& value
        );

        bool DrawStringField(
            TinyEngine::Component& component,
            const TinyEngine::FieldInfo& field,
            const std::any& value
        );

        bool DrawIntRectField(
            TinyEngine::Component& component,
            const TinyEngine::FieldInfo& field,
            const std::any& value
        );

        void DrawAddComponentMenu(TinyEngine::GameObject& gameObject);
	private:
        TinyEngine::ComponentRegister& componentRegister = TinyEngine::ComponentRegister::Instance();
		TinyEngine::Camera* editorCamera;
		TinyEngine::Window window;
		TinyEngine::TinyGameEngine engine;
		TinyEngine::TextureManager textureManager;
		std::unique_ptr< TinyEngine::Scene> editingScene;
		WorkingWindow workingWindow = WorkingWindow::Other;
		char renameBuffer[128]{};
		char fieldStringBuffer[256]{};
		const TinyEngine::FieldInfo* editingField = nullptr;
		TinyEngine::Component* editingComponent = nullptr;
		sf::Color sceneColor{ 55, 65, 80 };
		TinyEngine::GameObject* selectedObject = nullptr;
	};
}