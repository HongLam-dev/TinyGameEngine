#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Window.h"
#include "Input.h"
#include "GameObject.h"
#include "CollisionManager.h"

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
		GameObject& CreateGameObject();
		GameObject& CreateASimpleBox(const Vector3& position, const Vector3& size);
		float GetDeltaTime() const { return deltaTime; };
		float GetFixedDeltaTime() const { return 1.0/timeStep; };
	private:
		int targetFPS = 60;
		int timeStep = 60;
		float deltaTime = 0;
		CollisionManager collisionManager;
		sf::Clock clock;
		std::vector<std::unique_ptr<GameObject>> gameObjects;
	};
}