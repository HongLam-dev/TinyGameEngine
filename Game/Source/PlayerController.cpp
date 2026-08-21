#include "PlayerController.h"
#include "Input.h"
#include "vector3.h"
#include "GameObject.h"
#include "TinyGameEngine.h"
#include <iostream>

void PlayerController::Start() {
	transform = GetOwner().GetComponent<Transform>();
	rb = GetOwner().GetComponent<Rigidbody2D>();
}

void PlayerController::Update()
{
	if (Input::Get().isKeyPressed(sf::Keyboard::Key::W))
	{
		direction = Vector3::Down;
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