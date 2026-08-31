#pragma once
#include "Component.h"
#include "BoxCollider2D.h"
#include "RigidBody2D.h"
#include "Vector3.h"
#include "IComponentObserver.h"
#include <vector>
#include "Bounds.h"
#include <set>

namespace TinyEngine{

	class CollisionManager : public IComponentObserver
	{
    public:
        void CheckCollision(float fixedDeltaTime);
        void OnComponentAdded(Component& component) override;
        void OnComponentRemoved(Component& component) override;
        const std::vector<BoxCollider2D*>& GetColliders() const { return colliders; };
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

        struct ContinuousCollision
        {
            float enterTime;
            float exitTime;
            BoxCollider2D& a;
            BoxCollider2D& b;
            ContinuousCollision(float enter,float exit ,BoxCollider2D& a, BoxCollider2D& b):enterTime(enter), exitTime(exit), a(a), b(b)
            {}
        };

        const std::array<Collision,2>& CalculateCollisionAndResolveOverlap (BoxCollider2D& a,BoxCollider2D& b, const Vector3& aPos,const Vector3& bPos);
        bool CheckOverlapX(const Bounds& a, const Bounds& b);
        bool CheckOverlapY(const Bounds& a, const Bounds& b);
        ContinuousCollision ContinuousCollisionDetect(BoxCollider2D& a, BoxCollider2D& b, float fixedDeltaTime);
		void DiscreteCollisionDetect(BoxCollider2D& collider, BoxCollider2D& other);
        void ResolveCollision(BoxCollider2D& a, BoxCollider2D& b, const Vector3& correctionVector);
        void CollisionCallback(BoxCollider2D& a, BoxCollider2D& b,const Collision& aCollision, const Collision& bCollision);
        void TriggerCallback(BoxCollider2D& a, BoxCollider2D& b);
        void ExitCallback(BoxCollider2D& a, BoxCollider2D& b);

		std::vector<BoxCollider2D*> colliders;
        std::set<CollisionPair> previousPairs;
        std::set<CollisionPair> currentPairs;   
	};
}