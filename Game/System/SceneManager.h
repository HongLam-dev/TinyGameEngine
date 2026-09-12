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
		TinyEngine::Scene* GetActiveScene(){ return activeScene.get(); };
		void LoadExampleScene();
		void LoadScene(std::string sceneToLoad);
	private:
		std::vector<std::unique_ptr<TinyEngine::GameObject>> persistentObjects;
		std::unique_ptr<TinyEngine::Scene> activeScene=nullptr;
		TinyEngine::TinyGameEngine& engine;
		TinyEngine::TextureManager& textureManager;
		SceneBuilder sceneBuilder;
	};

}