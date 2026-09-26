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
	void SceneManager::LoadScene(std::string sceneToLoad) {
		if (!instance)
			throw std::runtime_error("No SceneManager instance exists");
		if (instance->sceneToLoad != "")
		{
			std::cout<<"Another scene loading is in :"<<instance->sceneToLoad<<'\n';
			return;
		}
		instance->sceneToLoad = sceneToLoad;
		instance->engine.EnqueueAction(instance->LoadNewScene);
	}

	SceneManager::~SceneManager()
	{
		instance = nullptr;
	}
	void SceneManager::LoadNewScene() {
		if (!instance)
			throw std::runtime_error("No SceneManager instance exists");
		instance->activeScene = std::make_unique<TinyEngine::Scene>(instance->engine);
		SceneSerializer::Instance().LoadScene(instance->sceneToLoad,
			*instance->activeScene);
		instance->engine.ActivateScene(*instance->activeScene);
		instance->sceneToLoad="";
	}
}