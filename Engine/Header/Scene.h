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
		Camera* GetMainCamera() { return mainCamera; };
		UIObject& CreateUIObject(TinyGameEngine& engine);
		GameObject& CreateMainCamera(TinyGameEngine& engine);
		GameObject& CreateCamera(TinyGameEngine& engine);
		std::unique_ptr<GameObject> RemoveSceneObject(GameObject& gameObject);
		void DestroySceneObject(GameObject& object);
		void Start();
		void Update(float deltaTime);
		void FixedUpdate(float fixedDeltaTime);
		void Unload();
	private:
		Camera* mainCamera = nullptr;
		std::vector<std::unique_ptr<GameObject>> sceneObjects;
	};

}