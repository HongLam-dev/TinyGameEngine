#pragma once
#include "Component.h"
#include "Vector3.h"

namespace TinyEngine {
	class Rigidbody2D: public Component
	{
	public:
		Rigidbody2D(GameObject& owner);
		void SetVelocity(const Vector3& velocity) { this->velocity = velocity; }
		void SetGravityScale(float factor) { gravityScale = factor; }
		Vector3 GetVelocity() { return velocity; }
		void Start() override;
		void SetMass(float mass) { this->mass = mass; }
		float GetMass() { return mass; }
		void ApplyGravity();
		void AddImpulse(Vector3 force);
		void AddForce(Vector3 force);
		void ApplyVelocity();
		void FixedUpdate() override;
	private:
		Vector3 velocity{0,0,0};
		Vector3 gravity{ 0.0f, 9.81f, 0.0f };
		float mass = 1.0f;
		float gravityScale = 1.0f;
		Vector3 accumulatedForce = Vector3::Zero;
	};
}