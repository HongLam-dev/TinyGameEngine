#include "RigidBody2D.h"
#include "Vector3.h"
#include "Transform.h"
#include "GameObject.h"
#include "TinyGameEngine.h"
#include <iostream>

namespace TinyEngine {

	Rigidbody2D::Rigidbody2D(GameObject& owner) :Component(owner) {
		
	};


	void Rigidbody2D::Start()
	{
		transform = &GetTransform();
		previousPostion = transform->GetPosition();
		std::vector<BoxCollider2D*> colliders = GetOwner().GetAllComponentsOfType<BoxCollider2D>();
		for (auto* collider : colliders)
		{
			collider->SetRigidbody();
		}
	}

	void Rigidbody2D::FixedUpdate(float fixedDeltaTime) {
		ApplyGravity();
		ApplyForce(fixedDeltaTime);
		ApplyVelocity(fixedDeltaTime);
	}

	void Rigidbody2D::ApplyVelocity(float fixedDeltaTime) {
		Vector3 newPosition = transform->GetPosition() + unappliedVelocity *fixedDeltaTime;
		MovePosition(newPosition);
		appliedVelocity = unappliedVelocity;
	}

	void Rigidbody2D::AddImpulse(Vector3 force) {

	}

	void Rigidbody2D::MovePosition(Vector3 pos) {
		previousPostion = transform->GetPosition();
		transform->SetPosition(pos);
	}

	void Rigidbody2D::AddForce(Vector3 force) {
		if (isKinematic)
			return;
		accumulatedForce += force;
	}
	void Rigidbody2D::ApplyGravity() {
		AddForce(gravity*gravityScale*mass);
	}
	void Rigidbody2D::ApplyForce(float fixedDeltaTime) {
		if (isKinematic)
			return;
		Vector3 acceleration = accumulatedForce / mass;

		unappliedVelocity += acceleration*fixedDeltaTime;
		accumulatedForce = Vector3::Zero;
	}
}