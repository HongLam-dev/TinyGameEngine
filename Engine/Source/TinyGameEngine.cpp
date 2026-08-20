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
		auto player = std::make_unique<GameObject>(*this);

		GameObject& playerRef = *player;

		sf::Texture texture;

		if (!texture.loadFromFile("Assets/player.png"))
			return;

		sf::Sprite sprite(texture);

		SpriteRenderer& renderer =
			playerRef.AddComponent<SpriteRenderer>();

		BoxCollider2D& collider =
			playerRef.AddComponent<BoxCollider2D>();

		collider.SetSize({
			PixelsToWorld(50),
			PixelsToWorld(50),
			PixelsToWorld(50)
			});

		collider.SetStatic(false);

		playerRef.AddComponent<PlayerController>();

		RigidBody2D& rb =
			playerRef.AddComponent<RigidBody2D>();

		rb.SetGravityFactor(0.1f);

		renderer.SetSprite(sprite);


		playerRef.GetComponent<Transform>()
			.SetPosition({
				PixelsToWorld(500),
				PixelsToWorld(-500),
				0
				});

		gameObjects.push_back(std::move(player));
		//end
		
		//player2
		auto player2 = std::make_unique<GameObject>(*this);

		GameObject& playerRef2 = *player2;

		sf::Texture texture2;

		if (!texture2.loadFromFile("Assets/player.png"))
			return;

		sf::Sprite sprite2(texture2);

		SpriteRenderer& renderer2 =
			playerRef2.AddComponent<SpriteRenderer>();

		BoxCollider2D& collider2 =
			playerRef2.AddComponent<BoxCollider2D>();

		collider2.SetSize({
			PixelsToWorld(50),
			PixelsToWorld(50),
			PixelsToWorld(50)
			});

		renderer2.SetSprite(sprite2);

		playerRef2.GetComponent<Transform>()
			.SetPosition({
				PixelsToWorld(500),
				PixelsToWorld(500),
				0
				});

		gameObjects.push_back(std::move(player2));
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
		collisonManager.CheckCollision();
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
		for (auto& collider : collisonManager.GetColliders())
		{
			window.DrawCollider(*collider);
		}

		window.Display();
	}

}