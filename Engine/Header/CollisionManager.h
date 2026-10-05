#pragma once
#include "Component.h"
#include "Collision.h"
#include "BoxCollider2D.h"
#include "RigidBody2D.h"
#include "Vector3.h"
#include <vector>
#include "Bounds.h"
#include <set>

namespace TinyEngine{
    constexpr float COLLISION_EPSILON = 1e-6f;
	class CollisionManager
	{
    public:
        void CheckCollision(float fixedDeltaTime);
        void RegisterCollider(BoxCollider2D& collider);
        void UnregisterCollider(BoxCollider2D& collider);
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

        struct CollisionResult
        {
            float enterTime;
            float exitTime;
            float t;
            BoxCollider2D* a;
            BoxCollider2D* b;
            bool hasCollision;
            Collision collisionA;
            Collision collisionB;
            CollisionResult(float enter,
                float exit ,
                float t,
                bool hasCollision,
                Collision collisionA, 
                Collision collisionB)
                :enterTime(enter), exitTime(exit),t(t), hasCollision(hasCollision), collisionA(collisionA), collisionB(collisionB)
            {
                a = collisionB.other;
                b = collisionA.other;
            }
        };

        bool CheckOverlapX(const Bounds& a, const Bounds& b, bool contactAsOverlap);
        bool CheckOverlapY(const Bounds& a, const Bounds& b, bool contactAsOverlap);
        Vector3 CalculateContactPoint(const Bounds& a, const Bounds& b);
        CollisionResult CalculateCollision(BoxCollider2D& a, BoxCollider2D& b, float fixedDeltaTime, float t);
        void ResolveRigidCollision(const CollisionResult& result, float fixedDeltaTime);
        void CollisionCallback(BoxCollider2D& a, BoxCollider2D& b,const Collision& aCollision, const Collision& bCollision);
        void TriggerCallback(BoxCollider2D& a, BoxCollider2D& b);
        void ExitCallback(BoxCollider2D& a, BoxCollider2D& b);

		std::vector<BoxCollider2D*> colliders;
        std::set<CollisionPair> previousPairs;
        std::set<CollisionPair> currentPairs;   
	};
}