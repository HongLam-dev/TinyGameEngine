#include "SceneBuilder.h"
#include "TinyGameEngine.h"
#include "Window.h"
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
#include "PingPong.h"
#include "CameraFollow.h"
#include "PlayerController.h"
using namespace TinyEngine;

namespace TinyGame {
	void SceneBuilder::BuildExampleScene1(TinyEngine::TinyGameEngine& engine,
		TinyEngine::Scene& emptyScene,
		TinyEngine::TextureManager& textureManager
	) {
		GameObject& playerRef = emptyScene.CreateSceneObject({ &engine.GetRenderManager() });

		sf::Texture* playerRunSheet=textureManager.GetTexture("Assets/PlayerRun.png");
		sf::Texture* placeHolderTex = textureManager.GetTexture("Assets/heart.png");

		if (!playerRunSheet)
			return;
		if (!placeHolderTex)
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
		renderer.SetTexture(*playerRunSheet);

		Transform& playerTranform = *playerRef.GetComponent<Transform>();
		playerTranform.SetPosition({
				PixelsToWorld(200),
				PixelsToWorld(300),
				0
			});
		playerTranform.SetScale({ 0.6f,0.6f,0.6f });

		//player animation
		Animation playerAni(*playerRunSheet);

		playerAni.AddKey({ {{ 53,38 },{ 100, 300 }},0 });
		playerAni.AddKey({ {{ 167,38 },{ 100, 300 }},0.1f });
		playerAni.AddKey({ {{ 311,38 },{ 120, 300 }},0.2f });
		playerAni.AddKey({ {{ 434,38 },{ 100, 300}},0.3f });
		playerAni.AddKey({ {{ 560,38 },{ 100, 300 }},0.4f });
		playerAni.AddKey({ {{ 675,38 },{ 140, 300 }},0.5f });

		Animator& animator = playerRef.AddComponent<Animator>();
		animator.AddAnimation("Run", std::move(playerAni));
		animator.SetAnimation("Run");
		//end

		//Create Image
		UIObject& imageObj = emptyScene.CreateUIObject({ &engine.GetRenderManager() });
		Image& image = imageObj.AddComponent<Image>();
		image.SetTexture(*placeHolderTex);
		Transform& imageTransform = imageObj.GetTransform();
		imageTransform.SetPosition({ 50, 500, 0 });
		//end image

		//Camera
		GameObject& camObj = emptyScene.CreateMainCamera({ &engine.GetRenderManager() });
		CameraFollow& camFollow = camObj.AddComponent<TinyGame::CameraFollow>();
		camFollow.SetTarget(playerTranform);

		//end camera
		//boxes
		sf::Texture* crateText=textureManager.GetTexture("Assets/Crate.png");

		if (!crateText)
			return;

		//CreateASimpleBox({400,200,0},{500,64,0});
		CreateASimpleBox(emptyScene, engine, { 400,300,0 }, { 64,64,0 }, crateText);
		CreateASimpleBox(emptyScene, engine, { 400,500,0 }, { 700,64,0 }, crateText);
		CreateAPingPongBox(emptyScene, engine, { 400,300,0 }, { 64,64,0 }, crateText);
		//end

	}

	void SceneBuilder::BuildExampleScene2(TinyEngine::TinyGameEngine& engine,
		TinyEngine::Scene& emptyScene,
		TinyEngine::TextureManager& textureManager
	) {
		sf::Texture* placeHolderTex = textureManager.GetTexture("Assets/heart.png");

		if (!placeHolderTex)
			return;
		//Create Image
		UIObject& imageObj = emptyScene.CreateUIObject({ &engine.GetRenderManager() });
		Image& image = imageObj.AddComponent<Image>();
		image.SetTexture(*placeHolderTex);
		Transform& imageTransform = imageObj.GetTransform();
		imageTransform.SetPosition({ 50, 500, 0 });
		//end image
		//boxes
		sf::Texture* crateText = textureManager.GetTexture("Assets/Crate.png");

		if (!crateText)
			return;

		//CreateASimpleBox({400,200,0},{500,64,0});
		CreateASimpleBox(emptyScene, engine, { 400,200,0 }, { 64,64,0 }, crateText);
		CreateASimpleBox(emptyScene, engine, { 400,600,0 }, { 700,64,0 }, crateText);
		CreateAPingPongBox(emptyScene, engine, { 400,100,0 }, { 64,64,0 }, crateText);
		CreateAPingPongBox(emptyScene, engine, { 400,500,0 }, { 64,64,0 }, crateText);
		//end

	}


	GameObject& SceneBuilder::CreateASimpleBox(Scene& scene,
		TinyEngine::TinyGameEngine& engine,
		const Vector3& position, 
		const Vector3& size, 
		sf::Texture* boxTexture) {
		GameObject& objectRef = scene.CreateSceneObject({ &engine.GetRenderManager() });

		if (boxTexture)
		{
			SpriteRenderer& renderer =
				objectRef.AddComponent<SpriteRenderer>();
			renderer.SetTexture(*boxTexture);
		}
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

	GameObject& SceneBuilder::CreateAPingPongBox(Scene& scene, TinyGameEngine& engine, const Vector3& position, const Vector3& size, sf::Texture* boxTexture) {
		GameObject& boxRef = CreateASimpleBox(scene, engine, position, size, boxTexture);

		PingPongAroundCenter& pingpong = boxRef.AddComponent<TinyGame::PingPongAroundCenter>();
		pingpong.Initialize({ 1,0,0 }, 3);

		boxRef.GetComponent<Transform>()
			->SetPosition({
				PixelsToWorld(400),
				PixelsToWorld(100),
				0
				});
		return boxRef;
	}

}