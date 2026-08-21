#include "RigidBody2D.h"
#include "Vector3.h"
#include "Transform.h"
#include "GameObject.h"
#include "TinyGameEngine.h"
#include <iostream>

namespace TinyEngine {
	void Rigidbody2D::FixedUpdate() {
		ApplyGravity();

		Vector3 acceleration = accumulatedForce / mass;

		velocity += acceleration;
		accumulatedForce = Vector3::Zero;

		ApplyVelocity();
	}

	void Rigidbody2D::ApplyVelocity() {
		velocity*= GetOwner().GetEngineContext().GetFixedDeltaTime();
		GetOwner().GetComponent<Transform>()->SetPosition(
			GetOwner().GetComponent<Transform>()->GetPosition()
			+ velocity 
		);
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