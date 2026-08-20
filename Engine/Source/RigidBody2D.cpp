#include "RigidBody2D.h"
#include "Vector3.h"
#include "Transform.h"
#include "GameObject.h"
#include "TinyGameEngine.h"
#include <iostream>

namespace TinyEngine {
	void RigidBody2D::FixedUpdate() {
		ApplyGravity();

		Vector3 acceleration = accumulatedForce / mass;

		velocity += acceleration;
		accumulatedForce = Vector3::Zero;

		ApplyVelocity();
	}

	void RigidBody2D::ApplyVelocity() {
		velocity*= GetOwner().GetEngineContext().GetFixedDeltaTime();
		GetOwner().GetComponent<Transform>().SetPosition(
			GetOwner().GetComponent<Transform>().GetPosition()
			+ velocity 
		);
	}


	void RigidBody2D::AddImpulse(Vector3 force) {

	}

	void RigidBody2D::AddForce(Vector3 force) {
		accumulatedForce += force;
	}
	void RigidBody2D::ApplyGravity() {
		AddForce(gravity*gravityFactor*mass);
	}
}