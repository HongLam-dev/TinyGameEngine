#pragma once
#include "Component.h"
#include "Vector3.h"

namespace TinyEngine {
	class RigidBody2D: public Component
	{
	public:
		RigidBody2D(GameObject& owner):Component(owner) {};
		void SetVelocity(const Vector3& velocity) { this->velocity = velocity; }
		void SetGravityFactor(float factor) { gravityFactor = factor; }
		Vector3 GetVelocity() { return velocity; }
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
		float gravityFactor = 1.0f;
		Vector3 accumulatedForce = Vector3::Zero;
	};
}