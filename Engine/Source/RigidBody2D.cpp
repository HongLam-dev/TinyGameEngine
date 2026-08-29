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
		transform = GetOwner().GetComponent<Transform>();
		previousPostion = transform->GetPosition();
		std::vector<BoxCollider2D*> colliders = GetOwner().GetAllComponentsOfType<BoxCollider2D>();
		for (auto* collider : colliders)
		{
			collider->SetRigidbody();
		}
	}

	void Rigidbody2D::FixedUpdate() {
		ApplyGravity();

		Vector3 acceleration = accumulatedForce / mass;

		velocity += acceleration;
		accumulatedForce = Vector3::Zero;

		ApplyVelocity();
	}

	void Rigidbody2D::ApplyVelocity() {
		Vector3 newPosition = GetTransform().GetPosition() + velocity * GetOwner().GetEngineContext().GetFixedDeltaTime();
		previousPostion = GetTransform().GetPosition();
		transform->SetPosition(newPosition);
	}

	void Rigidbody2D::AddImpulse(Vector3 force) {

	}

	void Rigidbody2D::AddForce(Vector3 force) {
		accumulatedForce += force;
	}
	void Rigidbody2D::ApplyGravity() {
		AddForce(gravity*gravityScale*mass);
	}
}