#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>
#include "Window.h"
#include "Input.h"
#include "GameObject.h"
#include "CollisionManager.h"
#include "Camera.h"
#include "UIObject.h"
#include "Scene.h"
#include "RenderableComponent.h"

namespace TinyEngine
{
	class TinyGameEngine
	{
	public:
		TinyGameEngine(Window& window):window(window) {};
		void FixedUpdate(float fixedDeltaTime);
		void Update(float deltaTime);
		void Render(TinyEngine::Window& window);
		void Run();
		void ActivateScene(Scene& sceneToActive);
		void RegisterRenderer(RenderableComponent& renderer);
		void UnregisterRenderer(RenderableComponent& renderer);
		void RegisterCollider(BoxCollider2D& collider);
		void UnregisterCollider(BoxCollider2D& collider);
		void DestroyGameObject(GameObject& gameObject);
		void DontDestroyOnload(GameObject& gameObject);
		RenderManager& GetRenderManager() { return renderManager; };
		void EnqueueAction(std::function<void()> action) { deferredActions.push_back(action); }
		void HandleReferredActions();
	private:
		Window& window;
		int targetFPS = 60;
		int timeStep = 60;
		float deltaTime = 0;
		RenderManager renderManager;
		Scene* activeScene=nullptr;
		std::vector<std::unique_ptr<GameObject>> persistentOjects;
		sf::Clock clock;
		std::vector<std::function<void()>> deferredActions;
		std::vector<GameObject*> objectsToDestroy;
		std::vector<GameObject*> objectsToMakePersistent;
	};
}