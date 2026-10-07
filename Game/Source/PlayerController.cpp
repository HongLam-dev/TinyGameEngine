#include "PlayerController.h"
#include "Input.h"
#include "vector3.h"
#include "GameObject.h"
#include "TinyGameEngine.h"
#include "BoxCollider2D.h"
#include "SceneManager.h"
#include <iostream>
using namespace TinyEngine;

namespace TinyGame {
	void PlayerController::Start() {
		transform = GetOwner().GetComponent<Transform>();
		rb = GetOwner().GetComponent<Rigidbody2D>();
		DontDestroyOnload(*this);
	}

	void PlayerController::Update(float deltaTime)
	{
		if (Input::Get().OnKeyDown(sf::Keyboard::Key::W))
		{
			//direction = Vector3::Down;
			rb->AddForce({ 0,-300,0 });
		}
		else if (Input::Get().isKeyPressed(sf::Keyboard::Key::D))
		{
			direction = Vector3::Right;
		}
		else if (Input::Get().isKeyPressed(sf::Keyboard::Key::S))
		{
			direction = Vector3::Up;
		}
		else if (Input::Get().isKeyPressed(sf::Keyboard::Key::A))
		{
			direction = Vector3::Left;
		}
		else if (Input::Get().isKeyPressed(sf::Keyboard::Key::K))
		{
			rb->AddForce({ 0,200,0 });
		}
		else if (Input::Get().isKeyPressed(sf::Keyboard::Key::I))
		{
			rb->AddForce({ 0,-200,0 });
		}
		else if (Input::Get().isKeyPressed(sf::Keyboard::Key::L))
		{
			rb->AddForce({ 20000,0,0 });
		}
		else if (Input::Get().isKeyPressed(sf::Keyboard::Key::J))
		{
			rb->AddForce({ -200,0,0 });
		}
		else if (Input::Get().isKeyPressed(sf::Keyboard::Key::G))
		{
			rb->SetGravityScale(1);
		}
		else if (Input::Get().isKeyPressed(sf::Keyboard::Key::H))
		{
			rb->SetGravityScale(0);
		}
		else
		{
			direction = Vector3::Zero;
		}

	}


	void PlayerController::FixedUpdate(float fixedDeltaTime)
	{
		if (rb != nullptr)
		{
			Vector3 velocity = direction * moveSpeed;
			velocity.y = rb->GetCurrentVelocity().y;
			rb->SetVelocity(velocity);
		}
	}

	void PlayerController::OnCollisionEnter(const Collision& collision) {
		std::cout <<"Normal: "<<collision.normal.x <<", " << collision.normal.y << " " << collision.other->GetOwner().GetName() << " Enter \n";
	}
	void PlayerController::OnCollisionStay(const Collision& collision) {
		std::cout << "Normal: " << collision.normal.x << ", " << collision.normal.y << " " << collision.other->GetOwner().GetName()<< " Stay \n";
	}
	void PlayerController::OnCollisionExit(BoxCollider2D& other) {
		std::cout <<other.GetOwner().GetName() << " Exit\n";
	}

	void PlayerController::OnTriggerEnter(BoxCollider2D& other) {
		std::cout << other.GetOwner().GetName() << " trigger Enter \n";
	}
	void PlayerController::OnTriggerStay(BoxCollider2D& other) {
		std::cout << other.GetOwner().GetName() <<" trigger Stay \n";
	}
	void PlayerController::OnTriggerExit(BoxCollider2D& other) {
		std::cout << other.GetOwner().GetName()<< " trigger  Exit \n";
	}
}