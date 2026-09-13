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

	void TinyGameEngine::Run()
	{
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
				HandleReferredActions();
			}
		}
	}
	void  TinyGameEngine::HandleReferredActions() {
		for (auto& action : deferredActions)
		{
			action();
		}
	}

	void TinyGameEngine::FixedUpdate(float fixedDeltaTime)
	{
		for (auto& object : persistentOjects)
		{
			object->FixedUpdate(fixedDeltaTime);
		}
		if(activeScene)
		{
			activeScene->FixedUpdate(fixedDeltaTime);
		}
	}

	void TinyGameEngine::Update(float deltaTime)
	{
		for (auto& object : persistentOjects)
		{
			object->Update(deltaTime);
		}

		if(activeScene)
		{
			activeScene->Update(deltaTime);
		}
	}

	void TinyGameEngine::Render(TinyEngine::Window& window)
	{
	
		window.Clear();
		if (activeScene)
		{
			Camera* mainCamera = activeScene->GetMainCamera();
			if (!mainCamera)
				return;

			renderManager.Render(window, *mainCamera);
			for (auto& collider : activeScene->GetCollidersInScene())
			{
				window.DrawCollider(*collider, *mainCamera);
			}
		}

		window.Display();
	}

	void TinyGameEngine::ActivateScene(Scene& scene) {
		activeScene = &scene;
		scene.Start();
	}

	void TinyGameEngine::RegisterRenderer(RenderableComponent& renderer) {
		renderManager.RegisterRenderer(renderer);
	}
	void TinyGameEngine::UnregisterRenderer(RenderableComponent& renderer) {
		renderManager.UnregisterRenderer(renderer);
	}
	void TinyGameEngine::RegisterCollider(BoxCollider2D& collider) {
		activeScene->RegisterCollider(collider);
	}
	void TinyGameEngine::UnregisterCollider(BoxCollider2D& collider) {
		activeScene->UnregisterCollider(collider);
	}

	void TinyGameEngine::DestroyGameObject(GameObject& gameObject) {
		auto it = std::find_if(
			persistentOjects.begin(),
			persistentOjects.end(),
			[&gameObject](const std::unique_ptr<GameObject>& c)
			{
				return c.get() == &gameObject;
			}
		);

		if (it != persistentOjects.end())
		{
			gameObject.OnDestroy();
			persistentOjects.erase(it);
		}
		else
		{
			activeScene->DestroySceneObject(gameObject);
		}
	}
}