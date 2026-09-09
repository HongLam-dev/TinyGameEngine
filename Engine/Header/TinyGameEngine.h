#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Window.h"
#include "Input.h"
#include "GameObject.h"
#include "CollisionManager.h"
#include "Camera.h"
#include "UIObject.h"
#include "RenderManager.h"

namespace TinyEngine
{
	class TinyGameEngine
	{
	public:
		void FixedUpdate();
		void Update();
		void Render(TinyEngine::Window& window);
		void Run(TinyEngine::Window& window);
		void StartObject();
		void SetCamera(Camera& camera) { this->mainCamera = &camera; };
		UIObject& CreateUIObject();
		GameObject& CreateGameObject();
		Camera& GetMainCamera() { return *mainCamera; };
		float GetDeltaTime() const { return deltaTime; };
		float GetFixedDeltaTime() const { return 1.0f/timeStep; };
	private:
		int targetFPS = 60;
		int timeStep = 60;
		float deltaTime = 0;
		Camera* mainCamera=nullptr;
		CollisionManager collisionManager;
		RenderManager renderManager;
		sf::Clock clock;
		std::vector<std::unique_ptr<GameObject>> gameObjects;
	};
}