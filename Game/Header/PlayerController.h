#pragma once
#include "Component.h"
#include "Vector3.h"
#include "Transform.h"
using namespace TinyEngine;

class PlayerController: public Component
{
public:
	using Component::Component;
	PlayerController(GameObject& owner):Component(owner) {}
	void Update() override;
	void FixedUpdate() override;
	void Start() override;
private:
	Transform* transform = nullptr;
	float moveSpeed=10.0f;
	Vector3 direction=Vector3::Zero;
};
