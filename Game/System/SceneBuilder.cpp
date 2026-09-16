#include "SceneBuilder.h"
#include "TinyGameEngine.h"
#include "Window.h"
#include <iostream>
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "BoxCollider2D.h"
#include "RigidBody2D.h"
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
		GameObject& playerRef = emptyScene.CreateSceneObject();

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

		collider.SetSize({0.64f,0.64f,0});
		playerRef.AddComponent<PlayerController>();

		Rigidbody2D& rb =
			playerRef.AddComponent<Rigidbody2D>();
		rb.SetGravityScale(0.0f);
		rb.SetCollisionDetectMode(CollisionDetectionMode::Continuous);
		//	collider.SetIsTrigger(true);
		renderer.SetTexture(*playerRunSheet);

		Transform& playerTranform = *playerRef.GetComponent<Transform>();
		playerTranform.SetPosition({2,3,0});
	//	playerTranform.SetScale({ 0.6f,0.6f,0.6f });

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
		UIObject& imageObj = emptyScene.CreateUIObject();
		Image& image = imageObj.AddComponent<Image>();
		image.SetTexture(*placeHolderTex);
		Transform& imageTransform = imageObj.GetTransform();
		imageTransform.SetPosition({ 50, 500, 0 });
		//end image

		//Camera
		GameObject& camObj = emptyScene.CreateMainCamera();
		CameraFollow& camFollow = camObj.AddComponent<TinyGame::CameraFollow>();
		camFollow.SetTarget(playerTranform);

		//end camera
		//boxes
		sf::Texture* crateText=textureManager.GetTexture("Assets/Crate.png");

		if (!crateText)
			return;

		//CreateASimpleBox({400,200,0},{500,64,0});
		emptyScene.CreateASimpleBox({ 4,3,0 }, { 10.0f,1.0f,0 }, crateText);
		emptyScene.CreateASimpleBox( { 4,1,0 }, { 0.64f,0.64f,0 }, crateText);
		CreateAPingPongBox(emptyScene, engine, { 4,0,0 }, { 0.64f,0.64f,0 }, crateText);
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
		UIObject& imageObj = emptyScene.CreateUIObject();
		Image& image = imageObj.AddComponent<Image>();
		image.SetTexture(*placeHolderTex);
		Transform& imageTransform = imageObj.GetTransform();
		imageTransform.SetPosition({ 0.5f, 5, 0 });
		//end image
		//Camera
		GameObject& camObj = emptyScene.CreateMainCamera();
		//end camera
		//boxes
		sf::Texture* crateText = textureManager.GetTexture("Assets/Crate.png");

		if (!crateText)
			return;

		//CreateASimpleBox({400,200,0},{500,64,0});
		emptyScene.CreateASimpleBox( {4,3,0 }, { 5.0f,0.64f,0 }, crateText);
		emptyScene.CreateASimpleBox( { 4,1,0 }, { 0.64f,0.64f,0 }, crateText);
	//	CreateAPingPongBox(emptyScene, engine, { 400,200,0 }, { 64,64,0 }, crateText);
	//	CreateAPingPongBox(emptyScene, engine, { 400,600,0 }, { 64,64,0 }, crateText);
		//end

	}

	GameObject& SceneBuilder::CreateAPingPongBox(Scene& scene, TinyGameEngine& engine, const Vector3& position, const Vector3& size, sf::Texture* boxTexture) {
		GameObject& boxRef = scene.CreateASimpleBox( position, size, boxTexture);

		PingPongAroundCenter& pingpong = boxRef.AddComponent<TinyGame::PingPongAroundCenter>();
		pingpong.Initialize({ 1,0,0 }, 3);

		boxRef.GetComponent<Transform>()
			->SetPosition({
				position.x,
				position.y,
				0
				});
		return boxRef;
	}

}