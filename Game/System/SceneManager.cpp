#include "SceneManager.h"
#include "TinyGameEngine.h"

namespace TGModule {
	SceneManager::SceneManager(TinyEngine::TinyGameEngine& engine):
		engine(engine){
		if (!instance)
			instance = this;
		else
			throw std::runtime_error("SceneManager already exists");

	}
	void SceneManager::LoadScene(std::string sceneToLoad)
	{
		if (!instance)
			throw std::runtime_error("No SceneManager instance exists");

		instance->engine.EnqueueAction(
			[sceneToLoad = std::move(sceneToLoad)]()
			{
				SceneManager::LoadNewScene(sceneToLoad);
			}
		);
	}

	SceneManager::~SceneManager()
	{
		instance = nullptr;
	}
	void SceneManager::LoadNewScene(const std::string& sceneToLoad)
	{
		if (!instance)
			throw std::runtime_error("No SceneManager instance exists");

		instance->activeScene =
			std::make_unique<TinyEngine::Scene>(instance->engine);

		SceneSerializer::Instance().LoadScene(
			sceneToLoad,
			*instance->activeScene
		);

		instance->engine.ActivateScene(*instance->activeScene);
	}
}