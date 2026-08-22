#pragma once
#include "Component.h"
#include "BoxCollider2D.h"
#include "RigidBody2D.h"
#include "Vector3.h"
#include "IComponentObserver.h"
#include <vector>
#include <set>

namespace TinyEngine{
	class CollisionManager : public IComponentObserver
	{
	private:
        struct CollisionPair
        {
            BoxCollider2D* a;
            BoxCollider2D* b;
            CollisionPair(BoxCollider2D* first, BoxCollider2D* second)
            {
                if (first < second)
                {
                    a = first;
                    b = second;
                }
                else
                {
                    a = second;
                    b = first;
                }
            }
            auto operator<=>(const CollisionPair&) const = default;
        };
	public:
		void CheckCollision();
		Vector3 CheckOverlap(const BoxCollider2D& collider, const BoxCollider2D& other);
		void ResolveCollision(BoxCollider2D& collider, BoxCollider2D& other, const Vector3& separation);
		const std::vector<BoxCollider2D*>& GetColliders() const { return colliders; };
		void OnComponentAdded(Component& component) override;
		void OnComponentRemoved(Component& component) override;
	private:
		std::vector<BoxCollider2D*> colliders;
        std::set<CollisionPair> previousPairs;
        std::set<CollisionPair> currentPairs;   
	};
}