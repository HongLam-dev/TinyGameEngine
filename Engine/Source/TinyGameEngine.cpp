#include "TinyGameEngine.h"
#include "Window.h"
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <iostream>
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "PlayerController.h"
namespace TinyEngine
{

	void TinyGameEngine::Run(TinyEngine::Window& window)
	{
		float accumulatedTimeStep = 0;
		float accumulatedRenderTime = 0;


		auto player = std::make_unique<GameObject>();

		GameObject& playerRef = *player;


		sf::Texture texture;
		if (!texture.loadFromFile("Assets/player.png"))
			return;
		sf::Sprite sprite(texture);
		SpriteRenderer& renderer = playerRef.AddComponent<SpriteRenderer>();
		playerRef.AddComponent<PlayerController>();

		renderer.SetSprite(sprite);

		gameObjects.push_back(std::move(player));

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

			ProcessInput();
			while (accumulatedTimeStep >= 1.0 / timeStep)
			{
				FixedUpdate();

				accumulatedTimeStep -= 1.0f / timeStep;
			}
			

			if (accumulatedRenderTime >= 1.0 / targetFPS)
			{
				Render(window);
				Update();
				accumulatedRenderTime = 0;
			}
		}
	}

	

	void TinyGameEngine::FixedUpdate()
	{
		for (auto& gameObject : gameObjects)
		{
			gameObject->FixedUpdate();
		}
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

		window.Display();
	}

	void TinyGameEngine::ProcessInput()
	{

	}
}