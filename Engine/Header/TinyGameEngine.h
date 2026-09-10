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
#include "SceneManager.h"

namespace TinyEngine
{
	class TinyGameEngine
	{
	public:
		void Start();
		void FixedUpdate(float fixedDeltaTime);
		void Update(float deltaTime);
		void Render(TinyEngine::Window& window);
		void Run(TinyEngine::Window& window);
		RenderManager& GetRenderManager() { return renderManager; };
		Scene* GetActiveScene() { return sceneManager.GetActiveScene(); }
	private:
		int targetFPS = 60;
		int timeStep = 60;
		float deltaTime = 0;
		RenderManager renderManager;
		SceneManager sceneManager;
		sf::Clock clock;
	};
}