#pragma once
#include "Component.h"
#include "BoxCollider2D.h"
#include "RigidBody2D.h"
#include "Vector3.h"
#include "IComponentObserver.h"
#include <vector>

namespace TinyEngine{
	class CollisionManager : public IComponentObserver
	{
	public:
		void CheckCollision();
		Vector3 CheckOverlap(const BoxCollider2D& collider, const BoxCollider2D& other);
		void AsignCollider(BoxCollider2D* collider) { colliders.push_back(collider); }
		void ResolveCollision(BoxCollider2D& collider, BoxCollider2D& other, const Vector3& separation);
		const std::vector<BoxCollider2D*>& GetColliders() const { return colliders; };
		void OnComponentAdded(Component& component) override;
		void OnComponentRemoved(Component& component) override;
	private:
		std::vector<BoxCollider2D*> colliders;
		std::vector<RigidBody2D*> rigidBodies;
	};
}