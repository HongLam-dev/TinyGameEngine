#include "Scene.h"
#include "GameObject.h"

namespace TinyEngine {
	GameObject& Scene::CreateSceneObject(std::vector<IComponentObserver*> observers)
	{
		auto go = std::make_unique<GameObject>();
		go->AddComponentObserver(collisionManager);
		for (auto& observer: observers)
		{
			go->AddComponentObserver(*observer);
		}

		sceneObjects.push_back(std::move(go));

		return *sceneObjects.back();
	}

	UIObject& Scene::CreateUIObject(std::vector<IComponentObserver*> observers)
	{
		auto go = std::make_unique<UIObject>();
		UIObject& uiObject = *go;
		for (auto& observer : observers)
		{
			go->AddComponentObserver(*observer);
		}
		sceneObjects.push_back(std::move(go));

		return uiObject;
	}

	void Scene::Start()
	{
		for (auto& gameObject : sceneObjects)
		{
			gameObject->Start();
		}
	}

	void Scene::FixedUpdate(float fixedDeltaTime)
	{
		for (auto& gameObject : sceneObjects)
		{
			gameObject->FixedUpdate(fixedDeltaTime);
		}
		collisionManager.CheckCollision(fixedDeltaTime);
	}

	void Scene::Update(float deltaTime)
	{
		for (auto& gameObject : sceneObjects)
		{
			gameObject->Update(deltaTime);
		}
	}
	GameObject& Scene::CreateCamera(std::vector<IComponentObserver*> observers) {
		GameObject& camObj = CreateSceneObject(observers);
		Camera& camera = camObj.AddComponent<Camera>();
		return camObj;
	}
	GameObject& Scene::CreateMainCamera(std::vector<IComponentObserver*> observers) {
		GameObject& camObj = CreateCamera(observers);
		SetCamera(*camObj.GetComponent<Camera>());
		return camObj;
	}
}