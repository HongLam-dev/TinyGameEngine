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

		Rigidbody2D* rba= a.GetRigidbody();
		Rigidbody2D* rbb= b.GetRigidbody();
		Vector3 aVelocity = rba ? a.GetRigidbody()->GetAppliedVelocity() : Vector3::Zero;
		Vector3 bVelocity =  rbb? b.GetRigidbody()->GetAppliedVelocity() : Vector3::Zero;

		float tEnter = fixedDeltaTime + 1;
		float tExit = fixedDeltaTime + 1;

		Vector3 relativeVelocity = aVelocity - bVelocity;

		Vector3 aPreviousPos = rba? rba->GetPreviousPosition():a.GetOwner().GetTransform().GetPosition();
		Vector3 bPreviousPos = rbb ? rbb->GetPreviousPosition() : b.GetOwner().GetTransform().GetPosition();
		Bounds aPreviousBounds = a.GetBoundsAtOwnerPosition(aPreviousPos);
		Bounds bPreviousBounds = b.GetBoundsAtOwnerPosition(bPreviousPos);

		bool overlapX = CheckOverlapX(a.GetBounds(), b.GetBounds(), false);
		bool overlapY = CheckOverlapY(a.GetBounds(), b.GetBounds(),false);

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

			tExit = std::min(tExitX, tExitY);

			if (tEnterX > tEnterY)
			{
				tEnter = tEnterX;
				normal.x = relativeVelocity.x > 0? -1:1;
			}
			else {
				tEnter = tEnterY;
				normal.y = relativeVelocity.y > 0 ? -1 : 1;
			}

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
		if (tEnter <= tExit && tExit > 0 && tEnter <= fixedDeltaTime)
		{
			hasCollision = true;

			Vector3 aCollidePos = aPreviousPos + aVelocity * (tEnter - t);
			Vector3 bCollidePos = bPreviousPos + bVelocity * (tEnter - t);

			contactPoint = CalculateContactPoint(a.GetBoundsAtOwnerPosition(aCollidePos), b.GetBoundsAtOwnerPosition(bCollidePos));
		}
		tEnter += t;
		tExit += t;
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
				Rigidbody2D* rba = a.GetRigidbody();
				Rigidbody2D* rbb = b.GetRigidbody();
				if (!rba && !rbb)
					continue;
				CollisionDetectionMode aMode = rba ?
					rba->GetCollisionDetectMode() : CollisionDetectionMode::Discrete;
				CollisionDetectionMode bMode = rbb ?
					rbb->GetCollisionDetectMode() : CollisionDetectionMode::Discrete;

				if (aMode == CollisionDetectionMode::Discrete && bMode == CollisionDetectionMode::Discrete)
				{
					Bounds ba = a.GetBounds();
					Bounds bb = b.GetBounds();
					bool overlap = CheckOverlapY(ba, bb, false) && CheckOverlapX(ba, bb, false);
					if (!overlap)
					{
						ExitCallback(a, b);
						continue;
					}
				}
				collisionResults.push_back(CalculateCollision(a, b, fixedDeltaTime, 0));
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
							std::vector<CollisionResult> recalculateResults;
							for (size_t i = 0; i < colliders.size(); i++)
							{
								for (size_t j = i + 1; j < colliders.size(); j++)
								{
									if (colliders[i] == &a || colliders[i] == &b ||
										colliders[j] == &a || colliders[j] == &b)
									{
										BoxCollider2D& c = *colliders[i];
										BoxCollider2D& d = *colliders[j];

										CollisionDetectionMode aMode = c.GetRigidbody() ?
											c.GetRigidbody()->GetCollisionDetectMode() : CollisionDetectionMode::Discrete;
										CollisionDetectionMode bMode = d.GetRigidbody() ?
											d.GetRigidbody()->GetCollisionDetectMode() : CollisionDetectionMode::Discrete;

										if (aMode == CollisionDetectionMode::Discrete && bMode == CollisionDetectionMode::Discrete)
										{
											Bounds bc = c.GetBounds();
											Bounds bd = d.GetBounds();
											bool overlap = CheckOverlapY(bc, bd, false) && CheckOverlapX(bc, bd, false);
											if (!overlap)
											{
												ExitCallback(c,d);
												continue;
											}
										}
										recalculateResults.push_back(CalculateCollision(c, d, fixedDeltaTime, t));
									}
								}
							}

							for (auto col = collisionResults.begin();
								col != collisionResults.end(); )
							{
								if (col->a == &a || col->a == &b ||
									col->b == &a || col->b == &b)
								{
									col = collisionResults.erase(col);
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
						if (!result.a->IsTrigger() && !result.b->IsTrigger())
						{
							ExitCallback(*result.a, *result.b);
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
	}

	void CollisionManager::ResolveRigidCollision(const CollisionResult& result,float fixedDeltaTime) {
		BoxCollider2D& a = *result.a;
		BoxCollider2D& b = *result.b;
		Rigidbody2D* rba = a.GetRigidbody();
		Rigidbody2D* rbb = b.GetRigidbody();

		Vector3 aVelocity = rba ? rba->GetAppliedVelocity() : Vector3::Zero;
		Vector3 bVelocity = rbb ? rbb->GetAppliedVelocity() : Vector3::Zero;

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
			a.SetColliderPosition(aCollidePos);
			b.SetColliderPosition(bCollidePos);
			rba->SetVelocity(aVelocity);
			rbb->SetVelocity(bVelocity);
			rba->ApplyVelocity(fixedDeltaTime - result.enterTime);
			rbb->ApplyVelocity(fixedDeltaTime - result.enterTime);
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
			a.SetColliderPosition(aCollidePos);
			rba->SetVelocity(aVelocity);
			rba->ApplyVelocity(fixedDeltaTime - result.enterTime);
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
			b.SetColliderPosition(bCollidePos);
			rbb->SetVelocity(bVelocity);
			rbb->ApplyVelocity(fixedDeltaTime - result.enterTime);
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

	bool CollisionManager::CheckOverlapX(
		const Bounds& a,
		const Bounds& b,
		bool contactAsOverlap)
	{
		float overlap =
			std::min(a.max.x, b.max.x) -
			std::max(a.min.x, b.min.x);

		if (contactAsOverlap)
			return overlap >= -COLLISION_EPSILON;

		return overlap > COLLISION_EPSILON;
	}

	bool CollisionManager::CheckOverlapY(
		const Bounds& a,
		const Bounds& b,
		bool contactAsOverlap)
	{
		float overlap =
			std::min(a.max.y, b.max.y) -
			std::max(a.min.y, b.min.y);

		if (contactAsOverlap)
			return overlap >= -COLLISION_EPSILON;

		return overlap > COLLISION_EPSILON;
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