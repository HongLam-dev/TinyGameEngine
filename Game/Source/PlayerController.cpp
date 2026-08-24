#include "PlayerController.h"
#include "Input.h"
#include "vector3.h"
#include "GameObject.h"
#include "TinyGameEngine.h"
#include "BoxCollider2D.h"
#include <iostream>

void PlayerController::Start() {
	transform = GetOwner().GetComponent<Transform>();
	rb = GetOwner().GetComponent<Rigidbody2D>();
}

void PlayerController::Update()
{
	if (Input::Get().isKeyPressed(sf::Keyboard::Key::W))
	{
		rb->AddForce({ 0,-20,0 });
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
	else
	{
		direction = Vector3::Zero;
	}

	if (Input::Get().isKeyPressed(sf::Keyboard::Key::G))
	{
		rb->SetGravityScale(1);
	}
}


void PlayerController::FixedUpdate()
{
	if (direction != Vector3::Zero && rb!=nullptr)
	{
		rb->SetVelocity(direction*moveSpeed);
	}
}

void PlayerController::OnCollisionEnter(const Collision& collision) {
	std::cout << "Enter";
}
void PlayerController::OnCollisionStay(const Collision& collision) {
	std::cout << "Stay";
}
void PlayerController::OnCollisionExit(const Collision& collision) {
	std::cout << "Exit";
}