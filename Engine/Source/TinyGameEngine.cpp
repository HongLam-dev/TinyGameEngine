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
		StartObject();
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
				Update();
			}

			while (accumulatedTimeStep >= 1.0 / timeStep)
			{
				FixedUpdate();

				accumulatedTimeStep -= 1.0f / timeStep;
			}

			if (deltaTime >= 1.0 / targetFPS)
			{
				Render(window);
				deltaTime = 0;
			}
		}
	}
	
	void TinyGameEngine::StartObject()
	{
		for (auto& gameObject : gameObjects)
		{
			gameObject->Start();
		}
	}

	void TinyGameEngine::FixedUpdate()
	{
		for (auto& gameObject : gameObjects)
		{
			gameObject->FixedUpdate();
		}
		collisionManager.CheckCollision(1.0f/timeStep);
	}

	void TinyGameEngine::Update()
	{
		for (auto& gameObject : gameObjects)
		{
			gameObject->Update();
		}
	}

	void TinyGameEngine::Render(TinyEngine::Window& window)
	{
	
		window.Clear();

		renderManager.Render(window,*mainCamera);
		for (auto& collider : collisionManager.GetColliders())
		{
			if (mainCamera)
				window.DrawCollider(*collider,*mainCamera);
		}

		window.Display();
	}
	GameObject& TinyGameEngine::CreateGameObject()
	{
		auto go = std::make_unique<GameObject>(*this);

		go->AddComponentObserver(collisionManager);
		go->AddComponentObserver(renderManager);

		gameObjects.push_back(std::move(go));

		return *gameObjects.back();
	}

	UIObject& TinyGameEngine::CreateUIObject()
	{
		auto go = std::make_unique<UIObject>(*this);
		UIObject& uiObject = *go;
		go->AddComponentObserver(renderManager);
		gameObjects.push_back(std::move(go));

		return uiObject;
	}

}