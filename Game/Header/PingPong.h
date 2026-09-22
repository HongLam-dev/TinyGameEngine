#pragma once
#include "Component.h"
#include "Vector3.h"
#include "Transform.h"
#include "RigidBody2D.h"
#include "BoxCollider2D.h"
namespace TinyGame {
	class PingPongAroundCenter : public TinyEngine::Component
	{
	public:
		using Component::Component;
		PingPongAroundCenter(TinyEngine::GameObject& owner) :
			Component(owner) {}
		void Update(float deltaTime) override;
		void FixedUpdate(float deltaTime) override;
		void Start() override;
		void Initialize(TinyEngine::Vector3 direction, float radius, float moveSpeed = 5.0f) {
			this->radius = radius;
			this->direction = direction;
			this->speed = moveSpeed;
		}
		float GetRadius() const;
		void SetRadius(float value);

		TinyEngine::Vector3 GetDirection() const;
		void SetDirection(TinyEngine::Vector3 value);

		float GetSpeed() const;
		void SetSpeed(float value);
	private:
		TinyEngine::Transform* transform = nullptr;
		float speed = 5.0f;
		TinyEngine::Vector3 direction = TinyEngine::Vector3::Zero;
		TinyEngine::Vector3 center = TinyEngine::Vector3::Zero;
		float radius = 5.0f;
	};
}