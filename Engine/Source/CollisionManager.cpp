#include "CollisionManager.h"
#include "BoxCollider2D.h"
#include "Vector3.h"
#include "Bounds.h"
#include "RigidBody2D.h"
#include "Collision.h"
#include "GameObject.h"
#include <array>
#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

namespace TinyEngine {

	void  CollisionManager::RegisterCollider(BoxCollider2D& collider) {
			colliders.push_back(&collider);
	}
	void  CollisionManager::UnregisterCollider(BoxCollider2D& collider) {
		auto it = std::find(colliders.begin(), colliders.end(), &collider);
		if (it != colliders.end())
		{
			colliders.erase(it);
		}
	}


	CollisionManager::CollisionResult CollisionManager::CalculateCollision(BoxCollider2D& a, BoxCollider2D& b, float fixedDeltaTime, float t)
	{
		Vector3 normal;
		Vector3 contactPoint;

		Vector3 aVelocity = a.GetRigidbody() ? a.GetRigidbody()->GetVelocity() : Vector3::Zero;
		Vector3 bVelocity = b.GetRigidbody() ? b.GetRigidbody()->GetVelocity() : Vector3::Zero;

		float tEnter = fixedDeltaTime + 1;
		float tExit = fixedDeltaTime + 1;

		Vector3 relativeVelocity = aVelocity - bVelocity;

		Vector3 aPreviousPos = a.GetPreviousWorldCenter();
		Vector3 bPreviousPos = b.GetPreviousWorldCenter();
		Bounds aPreviousBounds = a.GetBoundsAtPosition(aPreviousPos);
		Bounds bPreviousBounds = b.GetBoundsAtPosition(bPreviousPos);

		bool overlapX = CheckOverlapX(aPreviousBounds, bPreviousBounds,false);
		bool overlapY = CheckOverlapY(aPreviousBounds, bPreviousBounds,false);

		if (relativeVelocity.x !=0&& relativeVelocity.y!=0)
		{
			float tEnterX = tEnter;
			float tExitX = tExit;
			float tEnterY = tEnter;
			float tExitY = tExit;

			tEnterX = (bPreviousBounds.min.x - aPreviousBounds.max.x) / relativeVelocity.x;
			tExitX = (bPreviousBounds.max.x - aPreviousBounds.min.x) / relativeVelocity.x;
			tEnterY = (bPreviousBounds.min.y - aPreviousBounds.max.y) / relativeVelocity.y;
			tExitY = (bPreviousBounds.max.y - aPreviousBounds.min.y) / relativeVelocity.y;
			
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

			if (tEnterX > tEnterY)
			{
				tEnter = tEnterX;
				normal.x = relativeVelocity.x > 0? -1:1;
			}
			else {
				tEnter = tEnterY;
				normal.y = relativeVelocity.y > 0 ? -1 : 1;
			}

			tExit = std::min(tExitX, tExitY);
		}
		else {
			if (relativeVelocity.x == 0 && relativeVelocity.y!=0)
			{
				if (overlapX)
				{
					tEnter= (bPreviousBounds.min.y - aPreviousBounds.max.y) / relativeVelocity.y;
					tExit = (bPreviousBounds.max.y - aPreviousBounds.min.y) / relativeVelocity.y;

					if (relativeVelocity.y < 0)
					{
						float temp = tExit;
						tExit = tEnter;
						tEnter = temp;
						normal.y = 1;
					}
					else {
						normal.y = -1;
					}
				}
			}
			else if (relativeVelocity.y == 0 && relativeVelocity.x != 0)
			{
				if (overlapY)
				{
					tEnter = (bPreviousBounds.min.x - aPreviousBounds.max.x) / relativeVelocity.x;
					tExit = (bPreviousBounds.max.x - aPreviousBounds.min.x) / relativeVelocity.x;
					if (relativeVelocity.x < 0)
					{
						float temp = tExit;
						tExit = tEnter;
						tEnter = temp;
						normal.x = 1;
					}
					else {
						normal.x = -1;
					}
				}
			}
		}

		bool hasCollision = false;
		if (tEnter <= tExit && tExit > 0 && tEnter < fixedDeltaTime)
		{
			tEnter += t;
			hasCollision = true;

			Vector3 aCollidePos = aPreviousPos + aVelocity * (tEnter - t);
			Vector3 bCollidePos = bPreviousPos + bVelocity * (tEnter - t);

			Vector3 contactPoint = CalculateContactPoint(a.GetBoundsAtPosition(aCollidePos), b.GetBoundsAtPosition(bCollidePos));
			contactPoint = contactPoint;
			contactPoint = contactPoint;

		}
		Collision collisionA(b, normal, contactPoint, relativeVelocity);
		Collision collisionB(a, normal * -1, contactPoint, relativeVelocity * -1);
		return { tEnter, tExit,t, hasCollision, collisionA, collisionB };
	}

	void CollisionManager::CheckCollision(float fixedDeltaTime)
	{
		std::vector<CollisionResult> collisionResults;
		for (size_t i = 0; i < colliders.size(); i++)
		{
			for (size_t j = i + 1; j < colliders.size(); j++)
			{
				BoxCollider2D& a = *colliders[i];
				BoxCollider2D& b = *colliders[j];

				CollisionDetectionMode aMode = a.GetRigidbody() ?
					a.GetRigidbody()->GetCollisionDetectMode() : CollisionDetectionMode::Discrete;
				CollisionDetectionMode bMode = b.GetRigidbody() ?
					b.GetRigidbody()->GetCollisionDetectMode() : CollisionDetectionMode::Discrete;

				if (aMode == CollisionDetectionMode::Discrete && bMode == CollisionDetectionMode::Discrete)
					continue;
				collisionResults.push_back(CalculateCollision(a, b, fixedDeltaTime,0));
			}
		}

		if (!collisionResults.empty())
		{
			float t = 0;
			int trytime = 0;
			bool allResolved = false;
			while (!collisionResults.empty()&&!allResolved)
			{
				if (trytime > 100)
				{
					std::cout << "try time exceeded \n";
					break;
				}

				trytime++;

				std::sort(collisionResults.begin(), collisionResults.end(),
					[](const CollisionResult& a, const CollisionResult& b)
					{
						return a.enterTime < b.enterTime;
					});

				for (size_t i=0 ; i<collisionResults.size();i++ )
				{
					float nextEnterTime = fixedDeltaTime;

					for (size_t j = 0; j < collisionResults.size(); j++)
					{
						CollisionResult& result = collisionResults[j];
						if (result.hasCollision && result.enterTime > t)
						{
							nextEnterTime = result.enterTime;
							break;
						}
					}
					for (size_t j = 0; j < collisionResults.size(); j++)
					{
						CollisionResult& result = collisionResults[j];
						if (result.a->IsTrigger() || result.b->IsTrigger())
						{
							if (result.exitTime > t && result.exitTime <= nextEnterTime)
							{
								ExitCallback(*result.a, *result.b);
							}
						}
					}

					CollisionResult& result = collisionResults[i];
					if (result.hasCollision )
					{
						t = result.enterTime;
						BoxCollider2D& a = *result.a;
						BoxCollider2D& b = *result.b;

						if (!a.IsTrigger() && !b.IsTrigger())
						{
							ResolveRigidCollision(result, fixedDeltaTime);
							collisionResults.erase(collisionResults.begin() + i);
							std::vector<CollisionResult> recalculateResults;
							for (auto col = collisionResults.begin();
								col != collisionResults.end(); )
							{
								if (col->a == &a || col->a == &b ||
									col->b == &a || col->b == &b)
								{
									auto* colliderA = col->a;
									auto* colliderB = col->b;
									col = collisionResults.erase(col);
									auto newCollision =
										CalculateCollision(
											*colliderA,
											*colliderB,
											fixedDeltaTime,
											t);

									recalculateResults.push_back(newCollision);

								}
								else
								{
									++col;
								}
							}
							collisionResults.insert(collisionResults.end(), recalculateResults.begin(), recalculateResults.end());
							break;
						}
						else {
							TriggerCallback(a, b);
						}
					}
					else {
						if (!result.hasCollision)
						{
							if (!result.a->IsTrigger() && !result.b->IsTrigger())
							{
								if (!CheckOverlapX(result.a->GetBounds(), result.b->GetBounds(), false)
									|| !CheckOverlapY(result.a->GetBounds(), result.b->GetBounds(), false))
								{
									ExitCallback(*result.a, *result.b);
								}
							}
						}

					}
					if (i == collisionResults.size() - 1 || result.enterTime > fixedDeltaTime)
					{
						allResolved = true;
						break;
					}
				}
			
			}	

		}
	
		for (size_t i = 0; i < colliders.size(); i++)
		{
			for (size_t j = i + 1; j < colliders.size(); j++)
			{
				BoxCollider2D& a = *colliders[i];
				BoxCollider2D& b = *colliders[j];
				if (!a.GetRigidbody() && !b.GetRigidbody())
					continue;
				CollisionDetectionMode aMode = a.GetRigidbody() ? a.GetRigidbody()->GetCollisionDetectMode() : CollisionDetectionMode::Discrete;
				CollisionDetectionMode bMode = b.GetRigidbody() ? b.GetRigidbody()->GetCollisionDetectMode() : CollisionDetectionMode::Discrete;
				if (aMode == CollisionDetectionMode::Continuous || bMode == CollisionDetectionMode::Continuous)
					continue;

				DiscreteCollisionDetect(a, b,fixedDeltaTime);
			}
		}
		
	}

	void CollisionManager::ResolveRigidCollision(const CollisionResult& result,float fixedDeltaTime) {
		BoxCollider2D& a = *result.a;
		BoxCollider2D& b = *result.b;
		Rigidbody2D* rba = a.GetRigidbody();
		Rigidbody2D* rbb = b.GetRigidbody();

		Vector3 aVelocity = rba ? rba->GetVelocity() : Vector3::Zero;
		Vector3 bVelocity = rbb ? rbb->GetVelocity() : Vector3::Zero;

		Vector3 aPreviousPos = a.GetPreviousWorldCenter();
		Vector3 bPreviousPos = b.GetPreviousWorldCenter();

		Vector3 aCollidePos = aPreviousPos + aVelocity * (result.enterTime - result.t);
		Vector3 bCollidePos = bPreviousPos + bVelocity * (result.enterTime - result.t);

		CollisionCallback(a, b, result.collisionA, result.collisionB);

		float velocityAlongNormalA =result.collisionA.normal.Dot(aVelocity);
		float velocityAlongNormalB =result.collisionB.normal.Dot(bVelocity);

		if (rba && !rba->IsKinematic() && rbb && !rbb->IsKinematic())
		{
			if (velocityAlongNormalA < 0)
			{
				Vector3 normalVelocity = result.collisionA.normal * velocityAlongNormalA;
				aVelocity -= normalVelocity;
			}
			if (velocityAlongNormalB < 0)
			{
				Vector3 normalVelocity = result.collisionB.normal * velocityAlongNormalB;
				bVelocity -= normalVelocity;
			}
			a.SetPosition(aCollidePos);
			b.SetPosition(bCollidePos);
			rba->SetPosition(a.GetOwner().GetTransform().GetPosition() + (aVelocity * (fixedDeltaTime - result.enterTime )));
			rba->SetVelocity(aVelocity);
			rbb->SetPosition(b.GetOwner().GetTransform().GetPosition() + (bVelocity * (fixedDeltaTime - result.enterTime)));
			rbb->SetVelocity(bVelocity);
		}
		else if(rba && !rba->IsKinematic() && (rbb && rbb->IsKinematic())||!rbb)
		{
			Vector3 normalVelocityA = result.collisionA.normal * velocityAlongNormalA;
			Vector3 normalVelocityB = result.collisionB.normal * velocityAlongNormalB;
			if (velocityAlongNormalA < 0)
			{
				aVelocity -= normalVelocityA;
			}
			if (velocityAlongNormalB < 0)
			{
				aVelocity += normalVelocityB;
			}
			a.SetPosition(aCollidePos);
			rba->SetPosition(a.GetOwner().GetTransform().GetPosition() + (aVelocity * (fixedDeltaTime - result.enterTime)));
			rba->SetVelocity(aVelocity);
		} else if (rbb && !rbb->IsKinematic() && (rba && rba->IsKinematic()) || !rba)
		{
			Vector3 normalVelocityA = result.collisionA.normal * velocityAlongNormalA;
			Vector3 normalVelocityB = result.collisionB.normal * velocityAlongNormalB;
			if (velocityAlongNormalB < 0)
			{
				bVelocity -= normalVelocityB;
			}
			if (velocityAlongNormalA < 0)
			{
				bVelocity += normalVelocityA;
			}
			b.SetPosition(bCollidePos);
			rbb->SetPosition(b.GetOwner().GetTransform().GetPosition() + (bVelocity * (fixedDeltaTime - result.enterTime)));
			rbb->SetVelocity(bVelocity);
		}

	}

	void CollisionManager::DiscreteCollisionDetect(
		BoxCollider2D& a,
		BoxCollider2D& b , float fixedDeltaTime)
	{
		Bounds ba = a.GetBounds();
		Bounds bb = b.GetBounds();
		bool overlap = CheckOverlapY(ba, bb,false) && CheckOverlapX(ba, bb,false);
		if (overlap)
		{	
			if (!a.IsTrigger() && !a.IsTrigger())
			{
				CollisionResult result = CalculateCollision(a,b,fixedDeltaTime,0);

				ResolveRigidCollision(result,fixedDeltaTime);
				CollisionCallback(a, b,result.collisionA,result.collisionB);
			}
			else {
				TriggerCallback(a, b);
			}
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
			if (a.IsTrigger() || b.IsTrigger())
			{
				a.NotifyTriggerExit(b);
				b.NotifyTriggerExit(a);
			}
			else {
				a.NotifyCollisionExit(b);
				b.NotifyCollisionExit(a);
			}
			previousPairs = currentPairs;
		}
	}

	bool CollisionManager::CheckOverlapX(const Bounds& a, const Bounds& b, bool contactAsOverlap) {
		if (contactAsOverlap)
		{
			return a.min.x <= b.max.x &&
				a.max.x >= b.min.x;
		}
		else {
			return a.min.x < b.max.x &&
				a.max.x > b.min.x;
		}
		
	}
	bool CollisionManager::CheckOverlapY(const Bounds& a, const Bounds& b, bool contactAsOverlap) {
		if (contactAsOverlap)
		{
			return a.min.y <= b.max.y &&
				a.max.y >= b.min.y;
		}
		else {
			return a.min.y < b.max.y &&
				a.max.y > b.min.y;
		}
	}

	Vector3 CollisionManager::CalculateContactPoint(const Bounds& a, const Bounds& b) {
		Vector3 direction = a.center - b.center;
		Vector3 contactPoint;
		Vector3 separation;
		if (direction.x > 0)
		{
			separation.x = b.max.x - a.min.x;
			contactPoint.x = a.min.x + ((separation.x) / 2);
		}
		else
		{
			separation.x = b.min.x - a.max.x;
			contactPoint.x = a.min.x + ((separation.x) / 2);
		}

		if (direction.y > 0)
		{
			separation.y = b.max.y - a.min.y;
			contactPoint.y = a.min.y + ((separation.y) / 2);
		}
		else
		{
			separation.y = b.min.y - a.max.y;
			contactPoint.y = a.min.y + ((separation.y) / 2);
		}
		return contactPoint;
	}
}