#pragma once
#include "GameObject.h"
#include "CollisionManager.h"
#include "RenderManager.h"
#include "UI/UIObject.h"
#include "BoxCollider2D.h"
#include "RenderManager.h"
#include <vector>
namespace TinyEngine {
	class Scene {
	public:
		GameObject& CreateSceneObject(std::vector<IComponentObserver*> obervers);
		void SetCamera(Camera& camera) { this->mainCamera = &camera; };
		const std::vector<BoxCollider2D*>& GetCollidersInScene() const { return collisionManager.GetColliders(); };
		Camera* GetMainCamera() { return mainCamera; };
		UIObject& CreateUIObject(std::vector<IComponentObserver*> obervers);
		GameObject& CreateMainCamera(std::vector<IComponentObserver*> obervers);
		GameObject& CreateCamera(std::vector<IComponentObserver*> obervers);
		void Update(float deltaTime);
		void FixedUpdate(float fixedDeltaTime);
		void Start();
	private:
		CollisionManager collisionManager;
		Camera* mainCamera = nullptr;
		std::vector<std::unique_ptr<GameObject>> sceneObjects;
	};

}