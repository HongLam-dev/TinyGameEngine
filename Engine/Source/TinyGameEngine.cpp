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

		rb.SetGravityScale(0.1f);

		renderer.SetSprite(sprite);


		playerRef.GetComponent<Transform>()
			->SetPosition({
				PixelsToWorld(500),
				PixelsToWorld(-500),
				0
				});
		//end
		
		//player2
		GameObject& playerRef2 = CreateGameObject();

		sf::Texture texture2;

		if (!texture2.loadFromFile("Assets/player.png"))
			return;

		sf::Sprite sprite2(texture2);

		SpriteRenderer& renderer2 =
			playerRef2.AddComponent<SpriteRenderer>();

		BoxCollider2D& collider2 =
			playerRef2.AddComponent<BoxCollider2D>();

		collider2.SetSize({
			PixelsToWorld(64),
			PixelsToWorld(64),
			PixelsToWorld(0)
			});

		renderer2.SetSprite(sprite2);

		playerRef2.GetComponent<Transform>()
			->SetPosition({
				PixelsToWorld(500),
				PixelsToWorld(500),
				0
				});
		//end


		StartObject();

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
			fixedDeltatime += elapsedTime;

			if (deltaTime >= 1.0 / targetFPS)
			{	
				Update();
			}

			while (fixedDeltatime >= 1.0 / timeStep)
			{
				FixedUpdate();

				fixedDeltatime -= 1.0f / timeStep;
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
		collisionManager.CheckCollision();
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
}