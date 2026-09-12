#include "SceneManager.h"
#include "TinyGameEngine.h"

namespace TinyGame {
	SceneManager::SceneManager(TinyEngine::TinyGameEngine& engine, TinyEngine::TextureManager& textureManager):
		engine(engine),textureManager(textureManager) {
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
		instance->activeScene = std::make_unique<TinyEngine::Scene>();

		SceneBuilder::BuildExampleScene1(instance->engine,
			*instance->activeScene,
			instance->textureManager);
	}
	void SceneManager::LoadExampleScene2()
	{
		if (!instance)
			throw std::runtime_error("No SceneManager instance exists");
		instance->activeScene = std::make_unique<TinyEngine::Scene>();

		SceneBuilder::BuildExampleScene2(instance->engine,
			*instance->activeScene,
			instance->textureManager);
	}
}