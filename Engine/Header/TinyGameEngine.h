#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
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
		void Start();
		void FixedUpdate(float fixedDeltaTime);
		void Update(float deltaTime);
		void Render(TinyEngine::Window& window);
		void RunScene( Scene& sceneToRun);
		void RegisterRenderer(RenderableComponent& renderer);
		void UnregisterRenderer(RenderableComponent& renderer);
		void RegisterCollider(BoxCollider2D& collider);
		void UnregisterCollider(BoxCollider2D& collider);
		RenderManager& GetRenderManager() { return renderManager; };
	private:
		Window& window;
		int targetFPS = 60;
		int timeStep = 60;
		float deltaTime = 0;
		RenderManager renderManager;
		Scene* activeScene=nullptr;
		std::vector<std::unique_ptr<GameObject>> persistentOjects;
		sf::Clock clock;
	};
}