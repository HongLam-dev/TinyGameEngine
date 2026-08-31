#pragma once
#include "Component.h"
#include "Vector3.h"
#include "Transform.h"

namespace TinyEngine {
	enum class CollisionDetectionMode
	{
		Discrete,
		Continuous
	};

	class Rigidbody2D: public Component
	{
	public:
		Rigidbody2D(GameObject& owner);
		void Start() override;
		void ApplyGravity();
		void AddImpulse(Vector3 force);
		void AddForce(Vector3 force);
		void ApplyVelocity();
		void FixedUpdate() override;
		void SetVelocity(const Vector3& velocity) { this->velocity = velocity; }
		void SetGravityScale(float factor) { gravityScale = factor; }
		void SetMass(float mass) { this->mass = mass; }
		void SetPosition(Vector3 pos);
		void SetCollisionDetectMode(CollisionDetectionMode mode) { this->collisionDetectMode = mode; }
		Vector3 GetVelocity(){ return velocity; }
		float GetMass() { return mass; }
		CollisionDetectionMode GetCollisionDetectMode() { return collisionDetectMode; }
		Vector3 GetPreviousPosition() { return previousPostion; }
	private:
		Transform* transform=nullptr;
		Vector3 velocity{0,0,0};
		Vector3 gravity{ 0.0f, 9.81f, 0.0f };
		float mass = 1.0f;
		float gravityScale = 1.0f;
		Vector3 accumulatedForce = Vector3::Zero;
		Vector3 previousPostion = Vector3::Zero;
		CollisionDetectionMode collisionDetectMode = CollisionDetectionMode::Discrete;
	};
}