#include "SceneManager.h"

namespace TinyEngine {
	SceneManager::SceneManager() {
		LoadExampleScene();
	}

	void SceneManager::LoadExampleScene()
	{
		activeScene = std::make_unique<Scene>();		
	}

	void SceneManager::Update(float deltaTime) {
		for (auto& object : persistentObjects)
		{
			object->Update(deltaTime);
		}
		activeScene->Update(deltaTime);
	}
	void SceneManager::FixedUpdate(float fixedDeltaTime) {
		for (auto& object : persistentObjects)
		{
			object->FixedUpdate(fixedDeltaTime);
		}
		activeScene->FixedUpdate(fixedDeltaTime);
	}
	void SceneManager::Start() {
		for (auto& object : persistentObjects)
		{
			object->Start();
		}
		activeScene->Start();
	}
}