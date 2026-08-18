#pragma once
#include "Component.h"
#include "BoxCollider2D.h"
#include "Vector3.h"
#include <vector>

namespace TinyEngine{
	class CollisionManager
	{
	public:
		void CheckCollision();
		Vector3 CheckOverlap(const BoxCollider2D& collider, const BoxCollider2D& other);
		void AsignCollider(BoxCollider2D* collider) { colliders.push_back(collider); }
		void ResolveCollision(BoxCollider2D& collider, BoxCollider2D& other, const Vector3& separation);
	private:
		std::vector<BoxCollider2D*> colliders;
		
	};
}