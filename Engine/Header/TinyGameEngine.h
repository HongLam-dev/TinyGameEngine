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
		Camera& GetMainCamera() { return *mainCamera; };
		GameObject& CreateGameObject();
		UIObject& CreateUIObject();
		GameObject& CreateASimpleBox(const Vector3& position, const Vector3& size , sf::Texture* boxTexture = nullptr);
		GameObject& CreateAPingPongBox(const Vector3& position, const Vector3& size, sf::Texture* boxTexture = nullptr);
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