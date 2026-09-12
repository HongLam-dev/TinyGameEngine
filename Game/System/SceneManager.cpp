#include "SceneManager.h"
#include "TinyGameEngine.h"

namespace TinyGame {
	SceneManager::SceneManager(TinyEngine::TinyGameEngine& engine, TinyEngine::TextureManager& textureManager):
		engine(engine),textureManager(textureManager) {
	}

	void SceneManager::LoadExampleScene()
	{
		activeScene = std::make_unique<TinyEngine::Scene>();
		sceneBuilder.BuildExampleScene(engine,*activeScene,textureManager);
	}

}