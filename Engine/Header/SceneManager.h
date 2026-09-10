#pragma once
#include <vector>
#include "Scene.h"
#include "GameObject.h"

namespace TinyEngine {
	class SceneManager
	{
	public:
		void LoadScene(std::string sceneName);
		SceneManager();
		Scene* GetActiveScene(){ return activeScene.get(); };
		void LoadExampleScene();
		void Update(float deltaTime);
		void FixedUpdate(float fixedDeltaTime);
		void Start();
	private:
		std::vector<std::unique_ptr<GameObject>> persistentObjects;
		std::unique_ptr<Scene> activeScene=nullptr;
	};

}