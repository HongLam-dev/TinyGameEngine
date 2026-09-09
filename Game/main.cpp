#include "TinyGameEngine.h"
#include "GameObject.h"
#include "Window.h"
#include "Animation.h"
#include "PingPong.h"
#include "Image.h"
#include "Animator.h"
#include "CameraFollow.h"
#include "PlayerController.h"
#include "Camera.h"
#include "EngineSettings.h"
#include "Vector3.h"
using namespace TinyEngine;



GameObject& CreateASimpleBox(TinyGameEngine& engine, const Vector3& position, const Vector3& size, sf::Texture* boxTexture) {
	GameObject& objectRef = engine.CreateGameObject();

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

GameObject& CreateAPingPongBox(TinyGameEngine& engine, const Vector3& position, const Vector3& size, sf::Texture* boxTexture) {
GameObject& boxRef = CreateASimpleBox(engine,position, size, boxTexture);

	TinyGame::PingPongAroundCenter& pingpong = boxRef.AddComponent<TinyGame::PingPongAroundCenter>();
	pingpong.Initialize({ 1,0,0 }, 3);

	boxRef.GetComponent<Transform>()
		->SetPosition({
			PixelsToWorld(400),
			PixelsToWorld(100),
			0
			});
	return boxRef;
}


GameObject& CreateCamera(TinyGameEngine& engine) {
	GameObject& camObj = engine.CreateGameObject();
	Camera& camera = camObj.AddComponent<Camera>();
	return camObj;
}
GameObject& CreateMainCamera(TinyGameEngine& engine) {
	GameObject& camObj = CreateCamera(engine);
	engine.SetCamera(*camObj.GetComponent<Camera>());
	return camObj;
}

int main()
{
	TinyEngine::TinyGameEngine engine;
	TinyEngine::Window winow;


	GameObject& playerRef = engine.CreateGameObject();

	sf::Texture playerRunSheet;
	sf::Texture placeHolderTex;

	if (!playerRunSheet.loadFromFile("Assets/PlayerRun.png"))
		return 0;
	if (!placeHolderTex.loadFromFile("Assets/heart.png"))
		return 0;
	SpriteRenderer& renderer =
		playerRef.AddComponent<SpriteRenderer>();

	BoxCollider2D& collider =
		playerRef.AddComponent<BoxCollider2D>();

	collider.SetSize({
		PixelsToWorld(64),
		PixelsToWorld(64),
		PixelsToWorld(0)
		});
	playerRef.AddComponent<TinyGame::PlayerController>();

	Rigidbody2D& rb =
		playerRef.AddComponent<Rigidbody2D>();
	rb.SetGravityScale(0.0f);
	rb.SetCollisionDetectMode(CollisionDetectionMode::Continuous);
	//	collider.SetIsTrigger(true);
	renderer.SetTexture(playerRunSheet);

	Transform& playerTranform = *playerRef.GetComponent<Transform>();
	playerTranform.SetPosition({
			PixelsToWorld(200),
			PixelsToWorld(300),
			0
		});
	playerTranform.SetScale({ 0.6f,0.6f,0.6f });

	//player animation
	Animation playerAni(playerRunSheet);

	playerAni.AddKey({ {{ 53,38 },{ 100, 300 }},0 });
	playerAni.AddKey({ {{ 167,38 },{ 100, 300 }},0.1f });
	playerAni.AddKey({ {{ 311,38 },{ 120, 300 }},0.2f });
	playerAni.AddKey({ {{ 434,38 },{ 100, 300}},0.3f });
	playerAni.AddKey({ {{ 560,38 },{ 100, 300 }},0.4f });
	playerAni.AddKey({ {{ 675,38 },{ 140, 300 }},0.5f });

	Animator& animator = playerRef.AddComponent<Animator>();
	animator.SetAnimation(playerAni);
	//end

	//Create Image
	UIObject& imageObj = engine.CreateUIObject();
	Image& image = imageObj.AddComponent<Image>();
	image.SetTexture(placeHolderTex);
	Transform& imageTransform = imageObj.GetTransform();
	imageTransform.SetPosition({ 50, 500, 0 });
	//end image

	//Camera
	GameObject& camObj = CreateMainCamera(engine);
	TinyGame::CameraFollow& camFollow = camObj.AddComponent<TinyGame::CameraFollow>();
	camFollow.SetTarget(playerTranform);

	//end camera
	//boxes
	sf::Texture crateText;

	if (!crateText.loadFromFile("Assets/Crate.png"))
		return 0;

	//CreateASimpleBox({400,200,0},{500,64,0});
	CreateASimpleBox(engine,{ 400,300,0 }, { 64,64,0 }, &crateText);
	CreateASimpleBox(engine, { 400,500,0 }, { 700,64,0 }, &crateText);
	CreateAPingPongBox(engine, { 400,300,0 }, { 64,64,0 }, &crateText);
	//end


	engine.Run(winow);

	return 0;
}
