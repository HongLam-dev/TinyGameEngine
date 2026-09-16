#include "Scene.h"
#include "GameObject.h"

namespace TinyEngine {

	void  Scene::Unload() {
		for (auto& object : sceneObjects)
		{
			object->OnDestroy();
		}
	}
	void Scene::DestroySceneObject(GameObject& object) {
		auto it = std::find_if(
			sceneObjects.begin(),
			sceneObjects.end(),
			[&object](const std::unique_ptr<GameObject>& c)
			{
				return c.get() == &object;
			}
		);

		if (it != sceneObjects.end())
		{
			object.OnDestroy();
			sceneObjects.erase(it);
		}
	}
	std::unique_ptr<GameObject> Scene::RemoveSceneObject(GameObject& gameObject) {

		auto it = std::find_if(
			sceneObjects.begin(),
			sceneObjects.end(),
			[&gameObject](const std::unique_ptr<GameObject>& current)
			{
				return current.get() == &gameObject;
			});

		if (it == sceneObjects.end())
			return nullptr;

		std::unique_ptr<GameObject> result = std::move(*it);
		sceneObjects.erase(it);

		return result;
	}

	GameObject& Scene::CreateSceneObject()
	{
		auto go = std::make_unique<GameObject>(engine);
		go->SetName("NewGameObject");

		sceneObjects.push_back(std::move(go));

		return *sceneObjects.back();
	}

	UIObject& Scene::CreateUIObject()
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
	}

	void Scene::Update(float deltaTime)
	{
		for (auto& gameObject : sceneObjects)
		{
			gameObject->Update(deltaTime);
		}
	}


	GameObject& Scene::CreateASimpleBox(
		const Vector3& position,
		const Vector3& size,
		sf::Texture* boxTexture) {
		GameObject& objectRef = CreateSceneObject();

		if (boxTexture)
		{
			SpriteRenderer& renderer =
				objectRef.AddComponent<SpriteRenderer>();
			renderer.SetTexture(*boxTexture);
		}
		BoxCollider2D& collider =
			objectRef.AddComponent<BoxCollider2D>();

		collider.SetSize({
			size.x,
			size.y,
			size.z
			});

		objectRef.GetComponent<Transform>()
			->SetPosition({
				position.x,
				position.y,
				position.z
				});
		return objectRef;
	}


	GameObject& Scene::CreateCamera() {
		GameObject& camObj = CreateSceneObject();
		Camera& camera = camObj.AddComponent<Camera>();
		camObj.SetName("Camera");
		return camObj;
	}

	GameObject& Scene::CreateMainCamera() {
		GameObject& camObj = CreateCamera();
		SetCamera(*camObj.GetComponent<Camera>());
		camObj.SetName("Main Camera");
		return camObj;
	}

	const std::vector<std::unique_ptr<GameObject>>& Scene::GetGameObjects() const {
		return sceneObjects;
	}
}