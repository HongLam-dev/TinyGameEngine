#include "CollisionManager.h"
#include "BoxCollider2D.h"
#include "Vector3.h"
#include "Bounds.h"
#include <cmath>
#include <iostream>

namespace TinyEngine {

	void CollisionManager::OnComponentAdded(Component& component)
	{
		if (auto* collider = dynamic_cast<BoxCollider2D*>(&component))
		{
			AsignCollider(collider);
		}
	}
	void CollisionManager::OnComponentRemoved(Component& component) {

	}


	void CollisionManager::CheckCollision()
	{
		for (size_t i = 0; i < colliders.size(); i++)
		{
			for (size_t j = i+1; j < colliders.size(); j++)
			{
				BoxCollider2D& a = *colliders[i];
				BoxCollider2D& b = *colliders[j];
				if (a.IsStatic() && b.IsStatic())
					continue;
				Vector3 separation = CheckOverlap(a, b) ;
				if (separation != Vector3::Zero)
				{
					ResolveCollision(a,b, separation);
				}
			}
		}
	}

	void CollisionManager::ResolveCollision(
		BoxCollider2D& a,
		BoxCollider2D& b,
		const Vector3& separation)
	{
		if (a.IsStatic() && b.IsStatic())
			return;

		float aRatio;
		float bRatio;

		if (a.IsStatic())
		{
			aRatio = 0.0f;
			bRatio = 1.0f;
		}
		else if (b.IsStatic())
		{
			aRatio = 1.0f;
			bRatio = 0.0f;
		}
		else
		{
			aRatio = 0.5f;
			bRatio = 0.5f;
		}

		a.SetPosition(
			a.GetPosition() + separation * aRatio
		);

		b.SetPosition(
			b.GetPosition() - separation * bRatio
		);
	}

	Vector3 CollisionManager::CheckOverlap(
		const BoxCollider2D& collider,
		const BoxCollider2D& other)
	{
		Bounds a = collider.GetBounds();
		Bounds b = other.GetBounds();

		bool overlapX =
			a.min.x < b.max.x &&
			a.max.x > b.min.x;

		bool overlapY =
			a.min.y < b.max.y &&
			a.max.y > b.min.y;

		if (!overlapX || !overlapY)
			return Vector3::Zero;

		Vector3 direction =
			collider.GetPosition() - other.GetPosition();

		Vector3 separation;

		if (direction.x > 0)
			separation.x = b.max.x - a.min.x;
		else
			separation.x = b.min.x - a.max.x;

		if (direction.y > 0)
			separation.y = b.max.y - a.min.y;
		else
			separation.y = b.min.y - a.max.y;

		if (std::abs(separation.x) < std::abs(separation.y))
			separation.y = 0;
		else
			separation.x = 0;

		return separation;
	}
}