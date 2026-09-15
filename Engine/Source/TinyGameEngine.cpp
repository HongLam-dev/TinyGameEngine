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
			float elapsedTime = clock.restart().asSeconds();
			deltaTime += elapsedTime;
			accumulatedTimeStep += elapsedTime;

			while (const std::optional event = window.PollEvent())
			{
				if (event->is<sf::Event::Closed>())
					window.Close();
				Input::Get().ProcessEvent(*event);
			}

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
				window.Clear();
				Render(window,*activeScene->GetMainCamera());
				window.Display();

				HandleReferredActions();
				Input::Get().SyncKeyState();
				deltaTime = 0;
			}
		}
	}

	void TinyGameEngine::FixedUpdate(float fixedDeltaTime)
	{
		for (auto& object : persistentOjects)
		{
			object->FixedUpdate(fixedDeltaTime);
		}
		activeScene->FixedUpdate(fixedDeltaTime);
		collisionManager.CheckCollision(fixedDeltaTime);
	}

	void TinyGameEngine::Update(float deltaTime)
	{
		for (auto& object : persistentOjects)
		{
			object->Update(deltaTime);
		}
		activeScene->Update(deltaTime);
	}

	void TinyGameEngine::Render(TinyEngine::Window& window, Camera& camera) {
			renderManager.Render(window, camera);
			for (auto& collider : collisionManager.GetColliders())
			{
				window.DrawCollider(*collider, camera);
			}
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
		collisionManager.RegisterCollider(collider);
	}
	void TinyGameEngine::UnregisterCollider(BoxCollider2D& collider) {
		collisionManager.UnregisterCollider(collider);
	}

	void TinyGameEngine::DestroyGameObject(GameObject& gameObject) {
		objectsToDestroy.push_back(&gameObject);
	}

	void TinyGameEngine::DontDestroyOnload(GameObject& gameObject) {
		objectsToMakePersistent.push_back(&gameObject);
	}

	void  TinyGameEngine::HandleReferredActions() {
		for (auto& action : deferredActions)
		{
			action();
		}
		deferredActions.clear();

		for (auto& object : objectsToDestroy)
		{
			if (object)
			{
				auto it = std::find_if(
					persistentOjects.begin(),
					persistentOjects.end(),
					[&object](const std::unique_ptr<GameObject>& c)
					{
						return c.get() == object;
					}
				);

				if (it != persistentOjects.end())
				{
					object->OnDestroy();
					persistentOjects.erase(it);
				}
				else
				{
					activeScene->DestroySceneObject(*object);
				}
			}
		}

		objectsToDestroy.clear();

		for (auto& object : objectsToMakePersistent)
		{
			if(object)
				persistentOjects.push_back(std::move(activeScene->RemoveSceneObject(*object)));
		}

		objectsToMakePersistent.clear();
	}

}