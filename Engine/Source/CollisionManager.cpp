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

	bool CollisionManager::ContinuousCollisionDetect(BoxCollider2D& a, BoxCollider2D& b)
	{
		bool overlapped = false;
		Vector3 aVelocity;
		Vector3 bVelocity;
		Vector3 aPreviousPos;
		Vector3 bPreviousPos;

		if (a.GetRigidbody())
		{
			aVelocity=a.GetRigidbody()->GetVelocity();
			aPreviousPos = a.GetRigidbody()->GetPreviousPosition();
		}
		else
		{
			aVelocity = Vector3::Zero;
			aPreviousPos = a.GetPosition();
		}

		if (b.GetRigidbody())
		{
			bVelocity = b.GetRigidbody()->GetVelocity();
			bPreviousPos = b.GetRigidbody()->GetPreviousPosition();
		}
		else
		{
			bVelocity = Vector3::Zero;
			bPreviousPos = b.GetPosition();
		}
		Bounds aPreviousBounds = a.GetBoundsAtPosition(aPreviousPos);
		Bounds bPreviousBounds = b.GetBoundsAtPosition(bPreviousPos);
		Vector3 relativeVelocity = aVelocity - bVelocity;

		float tEnter = 0;
		float tExit = 1.0f / 60;

		if (relativeVelocity.x != 0 && relativeVelocity.y != 0)
		{
			float tEnterX = (bPreviousBounds.min.x - aPreviousBounds.max.x) / relativeVelocity.x;
			float tExitX = (bPreviousBounds.max.x - aPreviousBounds.min.x) / relativeVelocity.x;
			float tEnterY = (bPreviousBounds.min.y - aPreviousBounds.max.y) / relativeVelocity.y;
			float tExitY = (bPreviousBounds.max.y - aPreviousBounds.min.y) / relativeVelocity.y;

			if (tEnterX > 0 || tEnterY > 0)
			{
				if (tEnterX > tEnterY)
					tEnter = tEnterX;
				else
					tEnter = tEnterY;
			}

			if (tExitX < tExit || tExitY < tExit)
			{
				if (tExitX < tExitY)
					tExit = tExitX;
				else
					tExit = tExitY;
			}
		}
		else
		{
			if (relativeVelocity.x == 0 && CheckOverlapX(a.GetBounds(), a.GetBounds()))
			{
				float tEnterY = (bPreviousBounds.min.y - aPreviousBounds.max.y) / relativeVelocity.y;
				float tExitY = (bPreviousBounds.max.y - aPreviousBounds.min.y) / relativeVelocity.y;
				if (tEnterY > 0)
				{
					tEnter = tEnterY;
				}

				if (tExitY < tExit)
				{
					tExit = tExitY;
				}
			
			}
			else if (relativeVelocity.y == 0 && CheckOverlapY(a.GetBounds(), a.GetBounds()))
			{
				float tEnterX = (bPreviousBounds.min.x - aPreviousBounds.max.x) / relativeVelocity.x;
				float tExitX = (bPreviousBounds.max.x - aPreviousBounds.min.x) / relativeVelocity.x;
				if (tEnterX > 0)
				{
					tEnter = tEnterX;
				}

				if (tExitX < tExit)
				{
					tExit = tExitX;
				}
			}

		}

		if (tEnter < tExit)
		{
			overlapped = true;
			if (!a.GetIsTrigger() && !b.GetIsTrigger())
			{
				Vector3 aNewPos = aPreviousPos + aVelocity * tEnter;
				Vector3 bNewPos = bPreviousPos + bVelocity * tEnter;

				a.SetPosition(aNewPos);
				b.SetPosition(bNewPos);
				DiscreteCollisionDetect(a,b);
			}
		}

		return overlapped;
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

				Vector3 aVelocity = a.GetRigidbody()? a.GetRigidbody()->GetVelocity() : Vector3::Zero;
				Vector3 bVelocity = b.GetRigidbody() ? b.GetRigidbody()->GetVelocity() : Vector3::Zero;

				Vector3 relativeVeloctiy = aVelocity - bVelocity;
				CollisionDetectionMode aMode = a.GetRigidbody() ? a.GetRigidbody()->GetCollisionDetectMode() : CollisionDetectionMode::Discrete ;
				CollisionDetectionMode bMode = b.GetRigidbody() ? b.GetRigidbody()->GetCollisionDetectMode() : CollisionDetectionMode::Discrete;
				CollisionDetectionMode mode = aMode== CollisionDetectionMode::Discrete && bMode == CollisionDetectionMode::Discrete ?
					CollisionDetectionMode::Discrete : CollisionDetectionMode::Continuous;

				if(mode== CollisionDetectionMode::Discrete || relativeVeloctiy==Vector3::Zero)
					DiscreteCollisionDetect(a,b);
		 		else
					ContinuousCollisionDetect(a, b);
			}
		}
	}

	void CollisionManager::DiscreteCollisionDetect(
		BoxCollider2D& a,
		BoxCollider2D& b)
	{	
		Bounds ba = a.GetBounds();
		Bounds bb = b.GetBounds();
		bool overlap = CheckOverlapY(ba, bb) && CheckOverlapX(ba, bb);
		if (!a.GetIsTrigger() && !b.GetIsTrigger())
		{
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
				separation.x = bb.min.x- ba.max.x ;
				contactPoint.x =ba.min.x+((separation.x)/2);
			}

			if (direction.y > 0)
			{
				separation.y = bb.max.y - ba.min.y;
				contactPoint.y = ba.min.y + ((separation.y) / 2);
			}
			else
			{
				separation.y = bb.min.y- ba.max.y;
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
			Collision bCollision{ a,contactPoint,normal*(-1),relativeVeloctiy * (-1)};

			if (overlap)
			{
				ResolveCollision(a, b, correctionVector);
			}

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
	
		}
		previousPairs = currentPairs;
	}

	void CollisionManager::ResolveCollision(BoxCollider2D& a, BoxCollider2D& b, const Vector3& correctionVector) {

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
			);
	}


	bool CollisionManager::CheckOverlapX(const Bounds& a, const Bounds& b) {
		bool overlapX =
			a.min.x < b.max.x &&
			a.max.x > b.min.x;
		return overlapX;
	}
	bool CollisionManager::CheckOverlapY(const Bounds& a, const Bounds& b) {
		bool overlapY =
			a.min.y < b.max.y &&
			a.max.y > b.min.y;
		return overlapY;
	}
}