#pragma once
#include "TinyGameEngine.h"
#include "Window.h"
#include "Scene.h"
#include "InputHandler.h"
#include "EngineSettings.h"
#include "ComponentRegister.h"
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <iostream>
#include <TextureManager.h>

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
		void DrawTransform(TinyEngine::GameObject& gameObject);
		void DrawInspectorWindow(TinyEngine::GameObject*& selectedObject);
		void DrawMarker(sf::Vector2f  pixelPosition);
		void DrawObjectMarker(TinyEngine::GameObject*& selectedObject);
		void RegisterComponents();
	private:
		TinyEngine::ComponentRegister componentRegister;
		TinyEngine::Camera* editorCamera;
		TinyEngine::Window window;
		TinyEngine::TinyGameEngine engine;
		TinyEngine::TextureManager textureManager;
		std::unique_ptr< TinyEngine::Scene> editingScene;
		WorkingWindow workingWindow = WorkingWindow::Other;
		char renameBuffer[128]{};
		sf::Color sceneColor{ 55, 65, 80 };
		TinyEngine::GameObject* selectedObject = nullptr;
	};
}