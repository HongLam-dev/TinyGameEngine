#include "TinyGameEngine.h"
#include "Window.h"
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <iostream>
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "BoxCollider2D.h"
#include "RigidBody2D.h"
#include "EngineSettings.h"
#include "Animation.h"
#include "Animator.h"
#include "Camera.h"
#include "Image.h"
#include "UIObject.h"
namespace TinyEngine
{

	void TinyGameEngine::Run(TinyEngine::Window& window)
	{
		Start();
		float accumulatedTimeStep = 0;
		while (window.IsOpen())
		{
			while (const std::optional event = window.PollEvent())
			{
				if (event->is<sf::Event::Closed>())
					window.Close();
				Input::Get().ProcessEvent(*event);
			}

			float elapsedTime = clock.restart().asSeconds();
			deltaTime += elapsedTime;
			accumulatedTimeStep += elapsedTime;

			if (deltaTime >= 1.0 / targetFPS)
			{	
				Update(deltaTime);
			}

			while (accumulatedTimeStep >= 1.0 / timeStep)
			{
				FixedUpdate(1.0f/timeStep);

				accumulatedTimeStep -= 1.0f / timeStep;
			}

			if (deltaTime >= 1.0 / targetFPS)
			{
				Render(window);
				deltaTime = 0;
			}
		}
	}
	
	void TinyGameEngine::Start()
	{
		sceneManager.Start();
	}

	void TinyGameEngine::FixedUpdate(float fixedDeltaTime)
	{
		sceneManager.FixedUpdate(fixedDeltaTime);
	}

	void TinyGameEngine::Update(float deltaTime)
	{
		sceneManager.Update(deltaTime);
	}

	void TinyGameEngine::Render(TinyEngine::Window& window)
	{
	
		window.Clear();
		Scene* activeScene = sceneManager.GetActiveScene();
		if (!activeScene)
			return;

		Camera* mainCamera = activeScene->GetMainCamera();
		if (!mainCamera)
			return;

		renderManager.Render(window, *mainCamera);
		for (auto& collider : activeScene->GetCollidersInScene())
		{
			window.DrawCollider(*collider,*mainCamera);
		}

		window.Display();
	}

}