#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Window.h"
#include "Input.h"
#include "GameObject.h"

namespace TinyEngine
{
	class TinyGameEngine
	{
	public:
		void FixedUpdate();
		void Update();
		void Render(TinyEngine::Window& window);
		void ProcessInput();
		void Run(TinyEngine::Window& window, TinyEngine::Input& input);

	private:
		int targetFPS = 60;
		int timeStep = 60;
		float deltaTime = 0;
		sf::Clock clock;
		std::vector<GameObject> gameObjects;
	};
}