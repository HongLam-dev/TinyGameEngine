#pragma once
#include "Component.h"
#include "Vector3.h"
#include "Transform.h"
#include "RigidBody2D.h"
#include "BoxCollider2D.h"
namespace TinyGame {
	class PlayerController : public TinyEngine::Component
	{
	public:
		using Component::Component;
		PlayerController(TinyEngine::GameObject& owner) :Component(owner) {}
		void Update() override;
		void FixedUpdate() override;
		void Start() override;
		void OnCollisionEnter(const TinyEngine::Collision& collision) override;
		void OnCollisionStay(const TinyEngine::Collision& collision) override;
		void OnCollisionExit(TinyEngine::BoxCollider2D& other) override;
		void OnTriggerEnter(TinyEngine::BoxCollider2D& other) override;
		void OnTriggerStay(TinyEngine::BoxCollider2D& other) override;
		void OnTriggerExit(TinyEngine::BoxCollider2D& other) override;
	private:
		TinyEngine::Transform* transform = nullptr;
		TinyEngine::Rigidbody2D* rb = nullptr;
		float moveSpeed = 5.0f;
		TinyEngine::Vector3 direction = TinyEngine::Vector3::Zero;
	};
}