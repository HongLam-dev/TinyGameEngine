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
	SceneManager::~SceneManager()
	{
		instance = nullptr;
	}

	void SceneManager::LoadExampleScene1()
	{
		if (!instance)
			throw std::runtime_error("No SceneManager instance exists");
		instance->activeScene = std::make_unique<TinyEngine::Scene>(instance->engine);

		SceneSerializer::BuildExampleScene1(instance->engine,
			*instance->activeScene);
		instance->engine.ActivateScene(*instance->activeScene);
	}
	void SceneManager::LoadExampleScene2()
	{
		if (!instance)
			throw std::runtime_error("No SceneManager instance exists");
		instance->engine.EnqueueAction(LoadNewScene);
	}
	void SceneManager::LoadNewScene() {
		if (!instance)
			throw std::runtime_error("No SceneManager instance exists");
		instance->activeScene = std::make_unique<TinyEngine::Scene>(instance->engine);
		SceneSerializer::BuildExampleScene2(instance->engine,
			*instance->activeScene);
		instance->engine.ActivateScene(*instance->activeScene);
	}
}