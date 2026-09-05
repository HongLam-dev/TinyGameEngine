#pragma once
#include "Component.h"
#include "Vector3.h"
#include "Transform.h"
#include "RigidBody2D.h"
#include "BoxCollider2D.h"
using namespace TinyEngine;

class PingPongAroundCenter : public Component
{
public:
	using Component::Component;
	PingPongAroundCenter(GameObject& owner) :
		Component(owner) {}
	void Update() override;
	void FixedUpdate() override;
	void Start() override;
	void Initialize(Vector3 direction,float radius, float moveSpeed=5.0f) {
		this->radius = radius;
		this->direction = direction;
		this->moveSpeed = moveSpeed;
	}
	void OnCollisionEnter(const Collision& collision) override;
	void OnCollisionStay(const Collision& collision) override;
	void OnCollisionExit(BoxCollider2D& other) override;
	void OnTriggerEnter(BoxCollider2D& other) override;
	void OnTriggerStay(BoxCollider2D& other) override;
	void OnTriggerExit(BoxCollider2D& other) override;
private:
	Transform* transform = nullptr;
	float moveSpeed = 5.0f;
	Vector3 direction=Vector3::Zero;
	Vector3 center=Vector3::Zero;
	float radius=5.0f;
};
