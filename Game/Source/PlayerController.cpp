#include "PlayerController.h"
#include "Input.h"
#include "vector3.h"
#include "GameObject.h"
#include <iostream>

void PlayerController::Update()
{
	Vector3 targetPos;
	if (Input::Get().isKeyPressed(sf::Keyboard::Key::W))
	{
		targetPos = GetOwner().GetTransform().GetPosition();
		targetPos.y += moveSpeed;
		GetOwner().GetTransform().SetPosition(targetPos);
	}
	else if (Input::Get().isKeyPressed(sf::Keyboard::Key::D))
	{
		targetPos = GetOwner().GetTransform().GetPosition();
		targetPos.x += moveSpeed;
		GetOwner().GetTransform().SetPosition(targetPos);
	}
	else if (Input::Get().isKeyPressed(sf::Keyboard::Key::S))
	{
		targetPos = GetOwner().GetTransform().GetPosition();
		targetPos.y -= moveSpeed;
		GetOwner().GetTransform().SetPosition(targetPos);
	}
	else if (Input::Get().isKeyPressed(sf::Keyboard::Key::A))
	{
		targetPos = GetOwner().GetTransform().GetPosition();
		targetPos.x -= moveSpeed;
		GetOwner().GetTransform().SetPosition(targetPos);
	}
}

void PlayerController::FixedUpdate()
{

}