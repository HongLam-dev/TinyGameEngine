#include "CollisionManager.h"
#include "BoxCollider2D.h"
#include "Vector3.h"
#include "Bounds.h"
#include "RigidBody2D.h"
#include "Collision.h"
#include <cmath>
#include <iostream>

namespace TinyEngine {

	void CollisionManager::OnComponentAdded(Component& component)
	{
		if (auto* collider = dynamic_cast<BoxCollider2D*>(&component))
		{
			colliders.push_back(collider);
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
				if (!a.GetRigidbody() && !b.GetRigidbody())
					continue;
				CollisionCallback(a,b, CheckOverlap(a, b));
			}
		}
	}

	void CollisionManager::CollisionCallback(
		BoxCollider2D& a,
		BoxCollider2D& b,
		bool overlap)
	{		
		if (!a.GetIsTrigger() && !b.GetIsTrigger())
		{
			Bounds ba = a.GetBounds();
			Bounds bb = b.GetBounds();
			Vector3 direction = a.GetPosition() - b.GetPosition();
			Vector3 contactPoint;
			Vector3 separation;
			if (direction.x>0)
			{
				separation.x = bb.max.x - ba.min.x;
				contactPoint.x = ba.min.x+((separation.x)/2);
			}
			else
			{
				separation.x = ba.max.x - bb.min.x;
				contactPoint.x =ba.min.x+((separation.x)/2);
			}
			if (direction.y > 0)
			{
				separation.y = bb.max.y - ba.min.y;
				contactPoint.y = ba.min.y + ((separation.y) / 2);
			}
			else
			{
				separation.y = ba.max.y - bb.min.y;
				contactPoint.y = ba.min.y + ((separation.y) / 2);
			}

			Vector3 correctionVector = separation;
			if (std::abs(separation.x) < std::abs(separation.y))
				correctionVector.y = 0;
			else
				correctionVector.x = 0;

			Vector3 normal = correctionVector.Normalize();
			
			Vector3 aVelocity = a.GetRigidbody() ? a.GetRigidbody()->GetVelocity() : Vector3::Zero;
			Vector3 bVelocity = b.GetRigidbody()? b.GetRigidbody()->GetVelocity(): Vector3::Zero;

			Vector3 relativeVeloctiy = aVelocity - bVelocity;

			Collision aCollision{b,contactPoint,normal,relativeVeloctiy};
			Collision bCollision{ a,contactPoint,normal,relativeVeloctiy*(-1) };

			if (previousPairs.contains({ &a, &b }))
			{
				if (!overlap)
				{
					currentPairs.erase({ &a, &b });
					a.NotifyCollisionExit(aCollision);
					b.NotifyCollisionExit(bCollision);
				}
				else
				{
					a.NotifyCollisionStay(aCollision);
					b.NotifyCollisionStay(bCollision);
				}
			}
			else
			{
				if (overlap)
				{
					currentPairs.insert({ &a, &b });
					a.NotifyCollisionEnter(aCollision);
					b.NotifyCollisionEnter(bCollision);
				}
			}
			previousPairs = currentPairs;
		}

		else
		{
			if (previousPairs.contains({ &a, &b }))
			{
				if (!overlap)
				{
					currentPairs.erase({ &a, &b });
					a.NotifyTriggerExit(b);
					b.NotifyTriggerExit(a);
				}
				else
				{
					a.NotifyTriggerStay(b);
					b.NotifyTriggerStay(a);
				}
			}
			else
			{
				if (overlap)
				{
					currentPairs.insert({ &a, &b });
					a.NotifyTriggerEnter(b);
					b.NotifyTriggerEnter(a);
				}
			}
			previousPairs = currentPairs;
		}

	/*	Vector3 correctionVector = separation;
		if (std::abs(separation.x) < std::abs(separation.y))
			correctionVector.y = 0;
		else
			correctionVector.x = 0;

		float aRatio;
		float bRatio;

		if (!a.GetRigidbody())
		{
			aRatio = 0.0f;
			bRatio = 1.0f;
		}
		else if (!b.GetRigidbody())
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
			a.GetPosition() + correctionVector * aRatio
		);

		b.SetPosition(
			b.GetPosition() - correctionVector * bRatio
		);*/
	}

	bool CollisionManager::CheckOverlap(
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

		return overlapX && overlapY;
	}
}