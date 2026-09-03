#include "TinyGameEngine.h"
#include "Window.h"
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <iostream>
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "PlayerController.h"
#include "BoxCollider2D.h"
#include "RigidBody2D.h"
#include "EngineSettings.h"
namespace TinyEngine
{

	void TinyGameEngine::Run(TinyEngine::Window& window)
	{
		GameObject& playerRef = CreateGameObject();

		sf::Texture texture;

		if (!texture.loadFromFile("Assets/player.png"))
			return;

		sf::Sprite sprite(texture);

		SpriteRenderer& renderer =
			playerRef.AddComponent<SpriteRenderer>();

		BoxCollider2D& collider =
			playerRef.AddComponent<BoxCollider2D>();

		collider.SetSize({
			PixelsToWorld(64),
			PixelsToWorld(64),
			PixelsToWorld(0)
			});
		playerRef.AddComponent<PlayerController>();

		Rigidbody2D& rb =
			playerRef.AddComponent<Rigidbody2D>();

		rb.SetGravityScale(0.0f);
		rb.SetCollisionDetectMode(CollisionDetectionMode::Continuous);
	//	collider.SetIsTrigger(true);
		renderer.SetSprite(sprite);


		playerRef.GetComponent<Transform>()
			->SetPosition({
				PixelsToWorld(200),
				PixelsToWorld(100),
				0
				});
		//end
		
		//boxes
		CreateASimpleBox({200,200,0},{500,64,0});
		CreateASimpleBox({ 200,300,0 }, { 500,64,0 });
		CreateASimpleBox({ 200,400,0 }, { 500,64,0 });
		//CreateASimpleBox({ 600,500,0 }, { 64,64,0 });

		//end


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

		for (auto& gameObject : gameObjects)
		{
			gameObject->Render(window);
		}
		for (auto& collider : collisionManager.GetColliders())
		{
			window.DrawCollider(*collider);
		}

		window.Display();
	}
	GameObject& TinyGameEngine::CreateGameObject()
	{
		auto go = std::make_unique<GameObject>(*this);

		go->AddComponentObserver(collisionManager);

		gameObjects.push_back(std::move(go));

		return *gameObjects.back();
	}

	GameObject& TinyGameEngine::CreateASimpleBox(const Vector3& position, const Vector3& size) {
		GameObject& playerRef = CreateGameObject();

		BoxCollider2D& collider =
			playerRef.AddComponent<BoxCollider2D>();

		collider.SetSize({
			PixelsToWorld(size.x),
			PixelsToWorld(size.y),
			PixelsToWorld(size.z)
			});


		playerRef.GetComponent<Transform>()
			->SetPosition({
				PixelsToWorld(position.x),
				PixelsToWorld(position.y),
				position.z
				});
		return playerRef;
	}
}