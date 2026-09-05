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
#include "PingPong.h"
#include "Animation.h"
#include "Animator.h"
namespace TinyEngine
{

	void TinyGameEngine::Run(TinyEngine::Window& window)
	{
		GameObject& playerRef = CreateGameObject();

		sf::Texture texture;

		if (!texture.loadFromFile("Assets/PlayerRun.png"))
			return;
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
		renderer.SetTexture(texture);

		playerRef.GetComponent<Transform>()
			->SetPosition({
				PixelsToWorld(200),
				PixelsToWorld(300),
				0
				});
		playerRef.GetComponent<Transform>()
			->SetScale({0.6f,0.6f,0.6f});

		//player animation
		Animation playerAni(texture);

		playerAni.AddKey({{{ 53,38 },{ 100, 300 }},0});
		playerAni.AddKey({ {{ 167,38 },{ 100, 300 }},0.1f });
		playerAni.AddKey({ {{ 311,38 },{ 120, 300 }},0.2f });
		playerAni.AddKey({ {{ 434,38 },{ 100, 300}},0.3f });
		playerAni.AddKey({ {{ 560,38 },{ 100, 300 }},0.4f });
		playerAni.AddKey({ {{ 675,38 },{ 140, 300 }},0.5f });

		Animator& animator = playerRef.AddComponent<Animator>();
		animator.SetAnimation(playerAni);
		//end
		
		//boxes
		//CreateASimpleBox({400,200,0},{500,64,0});
		CreateASimpleBox({ 400,300,0 }, {64,64,0 });
		CreateASimpleBox({400,500,0 }, { 700,64,0 });
		CreateAPingPongBox({ 400,300,0 }, { 64,64,0 });
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
		GameObject& objectRef = CreateGameObject();

		BoxCollider2D& collider =
			objectRef.AddComponent<BoxCollider2D>();

		collider.SetSize({
			PixelsToWorld(size.x),
			PixelsToWorld(size.y),
			PixelsToWorld(size.z)
			});

		objectRef.GetComponent<Transform>()
			->SetPosition({
				PixelsToWorld(position.x),
				PixelsToWorld(position.y),
				position.z
				});
		return objectRef;
	}
	GameObject& TinyGameEngine::CreateAPingPongBox(const Vector3& position, const Vector3& size) {
		GameObject& playerRef = CreateGameObject();

		BoxCollider2D& collider =
			playerRef.AddComponent<BoxCollider2D>();

		collider.SetSize({
			PixelsToWorld(size.x),
			PixelsToWorld(size.y),
			PixelsToWorld(size.z)
			});
		PingPongAroundCenter& pingpong = playerRef.AddComponent<PingPongAroundCenter>();
		pingpong.Initialize({ 1,0,0 }, 3);

		playerRef.GetComponent<Transform>()
			->SetPosition({
				PixelsToWorld(400),
				PixelsToWorld(100),
				0
				});
		return playerRef;
	}
}