#include "CollisionManager.h"
#include "BoxCollider2D.h"
#include "Vector3.h"
#include "Bounds.h"
#include <cmath>
#include <iostream>

namespace TinyEngine{
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

	void CollisionManager::ResolveCollision(BoxCollider2D& collider,BoxCollider2D& other , const Vector3& separation)
	{
		Vector3 newPos;
		if (!collider.IsStatic())
		{
			newPos = collider.GetPosition();

			newPos += separation;

			collider.SetPosition(newPos);
		}	
		else
		{
			newPos = other.GetPosition();
			newPos -= separation;
			other.SetPosition(newPos);
		}
	}

	Vector3 CollisionManager::CheckOverlap(const BoxCollider2D& collider, const BoxCollider2D& other)
	{
		Vector3 separation=Vector3::Zero;
		Vector3 direction = collider.GetPosition() - other.GetPosition();
		Bounds boundsA = collider.GetBounds();
		Bounds boundsB = other.GetBounds();

		bool overlapX =
			boundsA.min.x < boundsB.max.x &&
			boundsA.max.x > boundsB.min.x;
		bool overlapY =
			boundsA.min.y < boundsB.max.y &&
			boundsA.max.y > boundsB.min.y;
		bool overlap =
			overlapX && overlapY;

		if (overlap)
		{
			if(direction.x>0)
				separation.x = boundsB.max.x - boundsA.min.x;
			else
			{
				separation.x = boundsB.min.x - boundsA.max.x;
			}
		}

		return separation;
	}
}