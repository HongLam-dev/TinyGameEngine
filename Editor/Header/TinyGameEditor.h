#pragma once
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

namespace TinyEditor {
	class TinyGameEditor
	{
	public:
		TinyGameEditor():engine(window) {};
		void Run();
		void DrawHierachyWindow(TinyEngine::GameObject*& selectedObject);
		void DrawTransform(TinyEngine::GameObject& gameObject);
		void DrawInspectorWindow(TinyEngine::GameObject*& selectedObject);
	private:
		TinyEngine::Window window;
		TinyEngine::TinyGameEngine engine;
		TinyEngine::TextureManager textureManager;
		std::unique_ptr< TinyEngine::Scene> editingScene;
	};
}