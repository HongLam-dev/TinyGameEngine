#pragma once
#include "Component.h"
#include "Vector3.h"
#include "Transform.h"
using namespace TinyEngine;

class PlayerController: public Component
{
public:
	PlayerController(GameObject& owner,float moveSpeed=10):Component(owner), moveSpeed(moveSpeed) {}
	void Update() override;
	void FixedUpdate() override;
	void Start() override;
private:
	Transform* transform = nullptr;
	float moveSpeed;
	Vector3 direction=Vector3::Zero;
};
