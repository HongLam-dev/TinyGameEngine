#pragma once
#include "TinyGameEngine.h"
#include "SceneSerializer.h"
#include "Window.h"
#include "Scene.h"
#include "InputHandler.h"
#include "EngineSettings.h"
#include "ComponentRegister.h"
#include "TextureManager.h"
#include "Component.h"
#include "Hierarchy.h"
#include "Inspector.h"
#include "ScenePreview.h"
#include "ProjectWindow.h"
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <iostream>
#include <functional>
#include <filesystem>

namespace TinyEditor {
	class TinyGameEditor
	{
	public:
		TinyGameEditor();
		void Run();
		void RegisterComponents();

        void CreateNewScene();
        void SaveScene();
        bool LoadScene(std::string sceneName);
		
		void DrawGadgetBar();

		void DuplicateObject();
	private:
        TinyEngine::ComponentRegister& componentRegister = TinyEngine::ComponentRegister::Instance();
		TinyEngine::Window window;
		TinyEngine::TinyGameEngine engine;

        Hierarchy hierarchy;
		ScenePreview scenePreview;
        Inspector inspector;
		ProjectWindow projectWindow;

		TinyEngine::Camera* editorCamera;
		std::unique_ptr< TinyEngine::Scene> editingScene;

		sf::Color sceneColor{ 55, 65, 80 };
		TinyEngine::GameObject* selectedObject = nullptr;

		std::string defaultSceneFolder = "Scenes";
		std::string sceneExtension = ".tge";
		std::string projectDirectory = "D:\\CodeProject\\TinyGameEngine\\Game";

		bool isGameRuning = false;
	};
}