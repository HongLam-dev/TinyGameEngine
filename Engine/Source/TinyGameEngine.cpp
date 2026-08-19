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
namespace TinyEngine
{

	void TinyGameEngine::Run(TinyEngine::Window& window)
	{
		float accumulatedTimeStep = 0;
		float accumulatedRenderTime = 0;

		auto player = std::make_unique<GameObject>(*this);

		GameObject& playerRef = *player;
		sf::Texture texture;
		if (!texture.loadFromFile("Assets/player.png"))
			return;
		sf::Sprite sprite(texture);
		SpriteRenderer& renderer = playerRef.AddComponent<SpriteRenderer>();
		BoxCollider2D& collider = playerRef.AddComponent<BoxCollider2D>();
		collider.SetStatic(false);
		collider.SetSize({ 50,50,50 });
		
		playerRef.AddComponent<PlayerController>();
		RigidBody2D& rb= playerRef.AddComponent<RigidBody2D>();
		rb.SetGravityFactor(0.01);

		renderer.SetSprite(sprite);
		playerRef.GetComponent<Transform>().SetScale({0.1f,0.1f,0.1f});

		gameObjects.push_back(std::move(player));

		//player2
		auto player2 = std::make_unique<GameObject>(*this);

		GameObject& playerRef2 = *player2;
		sf::Texture texture2;
		if (!texture2.loadFromFile("Assets/player.png"))
			return;
		sf::Sprite sprite2(texture2);
		SpriteRenderer& renderer2 = playerRef2.AddComponent<SpriteRenderer>();
		BoxCollider2D& collider2 = playerRef2.AddComponent<BoxCollider2D>();
		collider2.SetStatic(true);
		collider2.SetSize({ 50,50,50 });

		renderer2.SetSprite(sprite2);
		playerRef2.GetComponent<Transform>().SetScale({ 0.1f,0.1f,0.1f });
		playerRef2.GetComponent<Transform>().SetPosition({ 0,500,0 });

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

			deltaTime = clock.restart().asSeconds();
			accumulatedTimeStep += deltaTime;
			accumulatedRenderTime += deltaTime;

			if (accumulatedRenderTime >= 1.0 / targetFPS)
			{	
				Update();
			}

			while (accumulatedTimeStep >= 1.0 / timeStep)
			{
				FixedUpdate();

				accumulatedTimeStep -= 1.0f / timeStep;
			}

			if (accumulatedRenderTime >= 1.0 / targetFPS)
			{
				Render(window);
				accumulatedRenderTime = 0;
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