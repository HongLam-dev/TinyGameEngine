#include "PlayerController.h"
#include "Input.h"
#include "vector3.h"
#include "GameObject.h"
#include <iostream>

void PlayerController::Start() {
	transform = &GetOwner().GetComponent<Transform>();
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
}


void PlayerController::FixedUpdate()
{
	if (direction != Vector3::Zero)
	{
		Vector3 targetPos;
		targetPos = transform->GetPosition();
		targetPos += direction * moveSpeed;
		transform->SetPosition(targetPos);
	}
}