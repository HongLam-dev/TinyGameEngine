#pragma once
#include "GameObject.h"
#include "CollisionManager.h"
#include "RenderManager.h"
#include "UI/UIObject.h"
#include "BoxCollider2D.h"
#include <vector>
namespace TinyEngine {
	class TinyGameEngine;
	class Scene {
	public:
		Scene(TinyGameEngine& engine):engine(engine){}
		GameObject& CreateSceneObject();
		GameObject& CreateASimpleBox(
			const Vector3& position,
			const Vector3& size,
			sf::Texture* boxTexture=nullptr);
		void SetCamera(Camera& camera) { this->mainCamera = &camera; };
		Camera* GetMainCamera() { return mainCamera; };
		UIObject& CreateUIObject();
		GameObject& CreateMainCamera();
		GameObject& CreateCamera();
		std::unique_ptr<GameObject> RemoveSceneObject(GameObject& gameObject);
		void DestroySceneObject(GameObject& object);
		void Start();
		void Update(float deltaTime);
		void FixedUpdate(float fixedDeltaTime);
		void Unload();
		const std::vector<std::unique_ptr<GameObject>>& GetGameObjects() const;
	private:
		TinyGameEngine& engine;
		Camera* mainCamera = nullptr;
		std::vector<std::unique_ptr<GameObject>> sceneObjects;
	};

}