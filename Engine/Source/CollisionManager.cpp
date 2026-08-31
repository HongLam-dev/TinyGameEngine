#include "CollisionManager.h"
#include "BoxCollider2D.h"
#include "Vector3.h"
#include "Bounds.h"
#include "RigidBody2D.h"
#include "Collision.h"
#include <array>
#include <cmath>
#include <iostream>
#include <vector>'
#include <algorithm>

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

	CollisionManager::ContinuousCollision CollisionManager::ContinuousCollisionDetect(BoxCollider2D& a, BoxCollider2D& b, float fixedDeltaTime)
	{
		Vector3 aVelocity = a.GetRigidbody() ? a.GetRigidbody()->GetVelocity() : Vector3::Zero;
		Vector3 bVelocity = b.GetRigidbody() ? b.GetRigidbody()->GetVelocity() : Vector3::Zero;

		float tEnter = fixedDeltaTime + 1;
		float tExit = -1;

		Vector3 relativeVelocity = aVelocity - bVelocity;

		if (relativeVelocity != Vector3::Zero || CheckOverlapX(a.GetBounds(), b.GetBounds()) || CheckOverlapY(a.GetBounds(), b.GetBounds()))
		{
			bool overlapped = false;

			Vector3 aPreviousPos = a.GetRigidbody() ? a.GetRigidbody()->GetPreviousPosition() : a.GetPosition();
			Vector3 bPreviousPos = b.GetRigidbody() ? b.GetRigidbody()->GetPreviousPosition() : b.GetPosition();

			Bounds aPreviousBounds = a.GetBoundsAtPosition(aPreviousPos);
			Bounds bPreviousBounds = b.GetBoundsAtPosition(bPreviousPos);

			float tEnterX = relativeVelocity.x != 0 ? (bPreviousBounds.min.x - aPreviousBounds.max.x) / relativeVelocity.x : 0;
			float tExitX = relativeVelocity.x != 0 ? (bPreviousBounds.max.x - aPreviousBounds.min.x) / relativeVelocity.x : fixedDeltaTime+1;
			float tEnterY = relativeVelocity.y != 0 ? (bPreviousBounds.min.y - aPreviousBounds.max.y) / relativeVelocity.y : 0;
			float tExitY = relativeVelocity.y != 0 ? (bPreviousBounds.max.y - aPreviousBounds.min.y) / relativeVelocity.y : fixedDeltaTime+1;

			if (relativeVelocity.x < 0)
			{
				float temp = tExitX;
				tExitX = tEnterX;
				tEnterX = temp;
			}

			if (relativeVelocity.y < 0)
			{
				float temp = tExitY;
				tExitY = tEnterY;
				tEnterY = temp;
			}

			tEnter = tEnterX > tEnterY ? tEnterX : tEnterY;
			tExit = tExitX < tExitY ? tExitX : tExitY;
		}



		return ContinuousCollision(tEnter,tExit,a,b);
	}

	void CollisionManager::CheckCollision(float fixedDeltaTime)
	{
		/*std::vector<ContinuousCollision> collisionResults;
		for (size_t i = 0; i < colliders.size(); i++)
		{
			for (size_t j = i + 1; j < colliders.size(); j++)
			{
				BoxCollider2D& a = *colliders[i];
				BoxCollider2D& b = *colliders[j];
				CollisionDetectionMode aMode = a.GetRigidbody() ? a.GetRigidbody()->GetCollisionDetectMode() : CollisionDetectionMode::Discrete;
				CollisionDetectionMode bMode = b.GetRigidbody() ? b.GetRigidbody()->GetCollisionDetectMode() : CollisionDetectionMode::Discrete;
				if (aMode == CollisionDetectionMode::Discrete && bMode == CollisionDetectionMode::Discrete)
					continue;
				collisionResults.push_back(ContinuousCollisionDetect(a, b, fixedDeltaTime));
			}
		}

		if (!collisionResults.empty())
		{
			float t = 0;
			while (t<=fixedDeltaTime)
			{
				std::sort(collisionResults.begin(), collisionResults.end(),
					[](const ContinuousCollision& a, const ContinuousCollision& b)
					{
						return a.enterTime < b.enterTime;
					});
				for (auto& result : collisionResults)
				{
					if (result.enterTime < t)
						continue;
					t = result.enterTime;
					BoxCollider2D& a = result.a;
					BoxCollider2D& b = result.b;

					if (!a.GetIsTrigger() && !result.a.GetIsTrigger())
					{
						if (result.enterTime < fixedDeltaTime)
						{
							Vector3 aVelocity = a.GetRigidbody() ? a.GetRigidbody()->GetVelocity() : Vector3::Zero;
							Vector3 bVelocity = b.GetRigidbody() ? b.GetRigidbody()->GetVelocity() : Vector3::Zero;

							Vector3 aPreviousPos = a.GetRigidbody() ? a.GetRigidbody()->GetPreviousPosition() : a.GetPosition();
							Vector3 bPreviousPos = b.GetRigidbody() ? b.GetRigidbody()->GetPreviousPosition() : b.GetPosition();

							Vector3 aCollidePos = aPreviousPos + aVelocity * t;
							Vector3 bCollidePos = bPreviousPos + bVelocity * t;

							Bounds aPreviousBounds = a.GetBoundsAtPosition(aCollidePos);
							Bounds bPreviousBounds = b.GetBoundsAtPosition(bCollidePos);

							std::array<Collision, 2> collisions = CalculateCollisionAndResolveOverlap(a, b, aPreviousPos, bPreviousPos);
							float dotProductA = collisions[0].normal.Dot(aVelocity);
							float dotProductB = collisions[1].normal.Dot(bVelocity);
							if (dotProductA < 0 && a.GetRigidbody())
							{
								aVelocity = aVelocity +( aVelocity *= collisions[0].normal);
								a.GetRigidbody()->SetPosition(aPreviousPos + aVelocity*t);
								a.GetRigidbody()->SetVelocity(aVelocity);
							}
							if (dotProductA < 0 && b.GetRigidbody())
							{
								bVelocity = bVelocity + (bVelocity *= collisions[1].normal);
								b.GetRigidbody()->SetPosition(bPreviousPos + bVelocity * t);
								b.GetRigidbody()->SetVelocity(bVelocity);
							}
						}

					}
					else {

					}

				}
			}
		}
		*/
	
		for (size_t i = 0; i < colliders.size(); i++)
		{
			for (size_t j = i + 1; j < colliders.size(); j++)
			{
				BoxCollider2D& a = *colliders[i];
				BoxCollider2D& b = *colliders[j];
				DiscreteCollisionDetect(a, b);
			}
		}

	}

	const std::array<Collision, 2>& CollisionManager::CalculateCollisionAndResolveOverlap(BoxCollider2D& a, BoxCollider2D& b, const Vector3& aPos, const Vector3& bPos) {
		Vector3 direction = aPos - bPos;
		Vector3 contactPoint;
		Vector3 separation;
		Bounds ba = a.GetBoundsAtPosition(aPos);
		Bounds bb = b.GetBoundsAtPosition(bPos);
		if (direction.x > 0)
		{
			separation.x = bb.max.x - ba.min.x;
			contactPoint.x = ba.min.x + ((separation.x) / 2);
		}
		else
		{
			separation.x = bb.min.x - ba.max.x;
			contactPoint.x = ba.min.x + ((separation.x) / 2);
		}

		if (direction.y > 0)
		{
			separation.y = bb.max.y - ba.min.y;
			contactPoint.y = ba.min.y + ((separation.y) / 2);
		}
		else
		{
			separation.y = bb.min.y - ba.max.y;
			contactPoint.y = ba.min.y + ((separation.y) / 2);
		}

		Vector3 correctionVector = separation;
		if (std::abs(separation.x) < std::abs(separation.y))
			correctionVector.y = 0;
		else
			correctionVector.x = 0;

		ResolveCollision(a, b, correctionVector);

		Vector3 normal = correctionVector.Normalize();

		Vector3 aVelocity = a.GetRigidbody() ? a.GetRigidbody()->GetVelocity() : Vector3::Zero;
		Vector3 bVelocity = b.GetRigidbody() ? b.GetRigidbody()->GetVelocity() : Vector3::Zero;

		Vector3 relativeVeloctiy = aVelocity - bVelocity;

		Collision aCollision{ b,contactPoint,normal,relativeVeloctiy };
		Collision bCollision{ a,contactPoint,normal * (-1),relativeVeloctiy * (-1) };

		return { aCollision,bCollision };

	}

	void CollisionManager::DiscreteCollisionDetect(
		BoxCollider2D& a,
		BoxCollider2D& b)
	{
		Bounds ba = a.GetBounds();
		Bounds bb = b.GetBounds();
		bool overlap = CheckOverlapY(ba, bb) && CheckOverlapX(ba, bb);
		if (overlap)
		{
			std::array<Collision, 2> collisions = CalculateCollisionAndResolveOverlap(a, b, a.GetPosition(), b.GetPosition());
			CollisionCallback(a, b, collisions[0], collisions[1]);
		}
		else
		{
			ExitCallback(a, b);
		}

	}

	void CollisionManager::CollisionCallback(BoxCollider2D& a, BoxCollider2D& b, const Collision& aCollision, const Collision& bCollision) {
		if (previousPairs.contains({ &a, &b }))
		{
			a.NotifyCollisionStay(aCollision);
			b.NotifyCollisionStay(bCollision);
		}
		else
		{
			currentPairs.insert({ &a, &b });
			a.NotifyCollisionEnter(aCollision);
			b.NotifyCollisionEnter(bCollision);
		}
			
		previousPairs = currentPairs;
	}

	void CollisionManager::TriggerCallback(BoxCollider2D& a, BoxCollider2D& b) {
		
		currentPairs.insert({ &a, &b });
		if (previousPairs.contains({ &a, &b }))
		{
			a.NotifyTriggerStay(b);
			b.NotifyTriggerStay(a);
		}
		else
		{
			a.NotifyTriggerEnter(b);
			b.NotifyTriggerEnter(a);
		}

		previousPairs = currentPairs;
	}


	void CollisionManager::ExitCallback(BoxCollider2D& a, BoxCollider2D& b) {

		if (previousPairs.contains({ &a, &b }))
		{
			currentPairs.erase({ &a, &b });
			if (a.GetIsTrigger() || b.GetIsTrigger())
			{
				a.NotifyTriggerExit(b);
				b.NotifyTriggerExit(a);
			}
			else {
				a.NotifyCollisionExit(b);
				b.NotifyCollisionExit(a);
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