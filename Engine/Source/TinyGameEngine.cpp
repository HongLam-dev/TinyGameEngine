#include "TinyGameEngine.h"
#include "Window.h"
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <iostream>
#include "GameObject.h"
#include "SpriteRenderer.h"
namespace TinyEngine
{

	void TinyGameEngine::Run(TinyEngine::Window& window, TinyEngine::Input& input)
	{
		float accumulatedTimeStep = 0;
		float accumulatedRenderTime = 0;
		GameObject player;

		sf::Texture texture;
		if (!texture.loadFromFile("Assets/player.png"))
		{
			std::cout << "Failed \n";
			return;
		}
		sf::Sprite sprite(texture);
		SpriteRenderer& renderer = player.AddComponent<SpriteRenderer>();
		renderer.SetSprite(sprite);
		gameObjects.push_back(std::move(player));
		float moveSpeed = 20;
		Vector3 targetPos;

		while (window.IsOpen())
		{
			while (const std::optional event = window.PollEvent())
			{
				if (event->is<sf::Event::Closed>())
					window.Close();

				input.ProcessEvent(*event);
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

		if (input.isKeyPressed(sf::Keyboard::Key::W))
			{
				targetPos = player.GetTransform().GetPosition();
				targetPos.y += moveSpeed * deltaTime;
				player.GetTransform().SetPosition(targetPos);
			}
			else if (input.isKeyPressed(sf::Keyboard::Key::D))
			{
				targetPos = player.GetTransform().GetPosition();
				targetPos.x += moveSpeed * deltaTime;
				player.GetTransform().SetPosition(targetPos);
			}
			else if (input.isKeyPressed(sf::Keyboard::Key::S))
			{
				targetPos = player.GetTransform().GetPosition();
				targetPos.y -= moveSpeed * deltaTime;
				player.GetTransform().SetPosition(targetPos);
			}
			else if (input.isKeyPressed(sf::Keyboard::Key::A))
			{
				targetPos = player.GetTransform().GetPosition();
				targetPos.x -= moveSpeed * deltaTime;
				player.GetTransform().SetPosition(targetPos);
			}
			
			Update();

			if (accumulatedRenderTime >= 1.0 / targetFPS)
			{
				Render(window);

				accumulatedRenderTime = 0;
			}
		}
	}

	void TinyGameEngine::FixedUpdate()
	{
		for (auto& gameObject : gameObjects)
		{
			gameObject.FixedUpdate();
		}
	}

	void TinyGameEngine::Update()
	{
		for (auto& gameObject : gameObjects)
		{
			gameObject.Update();
		}
	}

	void TinyGameEngine::Render(TinyEngine::Window& window)
	{
	
		window.Clear();
		for (auto& gameObject : gameObjects)
		{
			gameObject.Render(window);
		}

		window.Display();
	}

	void TinyGameEngine::ProcessInput()
	{

	}
}