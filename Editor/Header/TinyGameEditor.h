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
		void DrawTransform(TinyEngine::Transform& transform);
		void DrawInspectorWindow(TinyEngine::GameObject*& selectedObject);
		void DrawMarker(sf::Vector2f  pixelPosition);
		void DrawObjectMarker(TinyEngine::GameObject*& selectedObject);
		void RegisterComponents();

        void DrawSelectedObject(TinyEngine::GameObject& gameObject);

        void DrawComponent(TinyEngine::Component& component);

        void DrawField(
            TinyEngine::Component& component,
            const TinyEngine::FieldInfo& field
        );

        void DrawFloatField(
            TinyEngine::Component& component,
            const TinyEngine::FieldInfo& field,
            const std::any& value
        );

        void DrawVector2Field(
            TinyEngine::Component& component,
            const TinyEngine::FieldInfo& field,
            const std::any& value
        );

        void DrawVector3Field(
            TinyEngine::Component& component,
            const TinyEngine::FieldInfo& field,
            const std::any& value
        );

        void DrawIntField(
            TinyEngine::Component& component,
            const TinyEngine::FieldInfo& field,
            const std::any& value
        );

        void DrawBoolField(
            TinyEngine::Component& component,
            const TinyEngine::FieldInfo& field,
            const std::any& value
        );

        void DrawStringField(
            TinyEngine::Component& component,
            const TinyEngine::FieldInfo& field,
            const std::any& value
        );

        void DrawIntRectField(
            TinyEngine::Component& component,
            const TinyEngine::FieldInfo& field,
            const std::any& value
        );

        void DrawAddComponentMenu(TinyEngine::GameObject& gameObject);
	private:
		TinyEngine::ComponentRegister componentRegister;
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