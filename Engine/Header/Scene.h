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
		GameObject& CreateSceneObject(TinyGameEngine& engine);
		void SetCamera(Camera& camera) { this->mainCamera = &camera; };
		const std::vector<BoxCollider2D*>& GetCollidersInScene() const { return collisionManager.GetColliders(); };
		Camera* GetMainCamera() { return mainCamera; };
		UIObject& CreateUIObject(TinyGameEngine& engine);
		GameObject& CreateMainCamera(TinyGameEngine& engine);
		GameObject& CreateCamera(TinyGameEngine& engine);
		void RegisterCollider(BoxCollider2D& collider);
		void UnregisterCollider(BoxCollider2D& collider);
		void Update(float deltaTime);
		void FixedUpdate(float fixedDeltaTime);
		void Start();
	private:
		CollisionManager collisionManager;
		Camera* mainCamera = nullptr;
		std::vector<std::unique_ptr<GameObject>> sceneObjects;
	};

}