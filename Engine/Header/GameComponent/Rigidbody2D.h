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
		void AddImpulse(Vector3 force);
		void AddForce(Vector3 force);

		void ApplyGravity();
		void ApplyForce(float fixedDeltaTime);
		void FixedUpdate(float fixedDeltaTime) override;
		void SetGravityScale(float factor) { gravityScale = factor; }
		void SetMass(float mass) { this->mass = mass; }
		void SetPosition(Vector3 pos);

		void SetVelocity(const Vector3& velocity) { this->velocity = velocity; }
		Vector3 GetVelocity(){ return velocity; }

		float GetMass() const { return mass; }
		float GetGravityScale() const { return gravityScale; }

		void SetCollisionDetectMode(CollisionDetectionMode mode) { this->collisionDetectMode = mode; }
		CollisionDetectionMode GetCollisionDetectMode() const { return collisionDetectMode; }

		bool IsKinematic() const { return isKinematic; }
		void SetKinematic(bool isKinematic) { this->isKinematic = isKinematic; }

		Vector3 GetPreviousWorldCenter() { return previousPostion; }
	private:
		void ApplyVelocity(float fixedDeltaTime);

		Transform* transform=nullptr;
		Vector3 velocity{0,0,0};
		Vector3 gravity{ 0.0f, 9.81f, 0.0f };
		float mass = 1.0f;
		float gravityScale = 1.0f;
		Vector3 accumulatedForce = Vector3::Zero;
		Vector3 previousPostion = Vector3::Zero;
		bool isKinematic=false;
		CollisionDetectionMode collisionDetectMode = CollisionDetectionMode::Discrete;
	};
}