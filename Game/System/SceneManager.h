#pragma once
#include <vector>
#include "Scene.h"
#include "GameObject.h"
#include "TinyGameEngine.h"
#include "SceneBuilder.h"
#include "TextureManager.h"

namespace TinyGame {
	class SceneManager
	{
	public:
		SceneManager(TinyEngine::TinyGameEngine& engine, TinyEngine::TextureManager& textureManager);
		~SceneManager();
		TinyEngine::Scene* GetActiveScene(){ return activeScene.get(); };
		static void LoadExampleScene1();
		static void LoadExampleScene2();
		static void LoadScene(std::string sceneToLoad);
	private:
		std::vector<std::unique_ptr<TinyEngine::GameObject>> persistentObjects;
		std::unique_ptr<TinyEngine::Scene> activeScene=nullptr;
		TinyEngine::TinyGameEngine& engine;
		TinyEngine::TextureManager& textureManager;
		inline static SceneManager* instance = nullptr;

	};

}