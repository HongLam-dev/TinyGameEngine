#pragma once
#include <vector>
#include "Scene.h"
#include "GameObject.h"
#include "TinyGameEngine.h"
#include "SceneBuilder.h"
#include "TextureManager.h"
#include "RenderableComponent.h"

namespace TinyGame {
	class SceneManager
	{
	public:
		SceneManager(TinyEngine::TinyGameEngine& engine);
		~SceneManager();
		static void LoadExampleScene1();
		static void LoadExampleScene2();
		static void LoadNewScene();
		static void LoadScene(std::string sceneToLoad);
	private:
		std::vector<std::unique_ptr<TinyEngine::GameObject>> persistentObjects;
		std::unique_ptr<TinyEngine::Scene> activeScene=nullptr;
		TinyEngine::TinyGameEngine& engine;
		inline static SceneManager* instance = nullptr;

	};

}