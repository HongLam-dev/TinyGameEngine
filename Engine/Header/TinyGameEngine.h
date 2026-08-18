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
		CollisionManager& GetCollisionManager() { return collisonManager; }

	private:
		int targetFPS = 60;
		int timeStep = 60;
		float deltaTime = 0;
		CollisionManager collisonManager;
		sf::Clock clock;
		std::vector<std::unique_ptr<GameObject>> gameObjects;
	};
}