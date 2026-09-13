#include "Scene.h"
#include "GameObject.h"

namespace TinyEngine {

	void Scene::RegisterCollider(BoxCollider2D& collider) {
		collisionManager.RegisterCollider(collider);
	}
	void Scene::UnregisterCollider(BoxCollider2D& collider) {
		collisionManager.UnregisterCollider(collider);
	}
	GameObject& Scene::CreateSceneObject(TinyGameEngine& engine)
	{
		auto go = std::make_unique<GameObject>(engine);
		sceneObjects.push_back(std::move(go));

		return *sceneObjects.back();
	}

	UIObject& Scene::CreateUIObject(TinyGameEngine& engine)
	{
		auto go = std::make_unique<UIObject>(engine);
		UIObject& uiObject = *go;
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
	GameObject& Scene::CreateCamera(TinyGameEngine& engine) {
		GameObject& camObj = CreateSceneObject(engine);
		Camera& camera = camObj.AddComponent<Camera>();
		return camObj;
	}
	GameObject& Scene::CreateMainCamera(TinyGameEngine& engine) {
		GameObject& camObj = CreateCamera(engine);
		SetCamera(*camObj.GetComponent<Camera>());
		return camObj;
	}
}