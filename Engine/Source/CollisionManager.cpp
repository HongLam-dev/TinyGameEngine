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


	CollisionManager::ContinuousCollision CollisionManager::ContinuousCollisionDetect(BoxCollider2D& a, BoxCollider2D& b, float fixedDeltaTime, float t)
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
		}
		Collision collisionA(b, normal, contactPoint, relativeVelocity);
		Collision collisionB(a, normal * -1, contactPoint, relativeVelocity * -1);
		return { tEnter, tExit,t, hasCollision, collisionA, collisionB };
	}

	void CollisionManager::CheckCollision(float fixedDeltaTime)
	{
		std::vector<ContinuousCollision> collisionResults;
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
				collisionResults.push_back(ContinuousCollisionDetect(a, b, fixedDeltaTime,0));
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
					[](const ContinuousCollision& a, const ContinuousCollision& b)
					{
						return a.enterTime < b.enterTime;
					});
				for (size_t i=0 ; i<collisionResults.size();i++ )
				{
					bool recalculated = false;
					ContinuousCollision& result = collisionResults[i];
					if (!result.hasCollision || result.enterTime < t)
					{
						ExitCallback(*result.a, *result.b);		
						if (i == collisionResults.size() - 1|| result.enterTime > fixedDeltaTime)
						{
							t = result.enterTime;
							allResolved = true;
							break;
						}
						continue;
					}

				/**	for (size_t j = 0; j < collisionResults.size(); j++)
					{
						if (collisionResults[j].a->IsTrigger() || collisionResults[j].b->IsTrigger())
						{
							if (collisionResults[j].exitTime >= t && collisionResults[j].exitTime < result.enterTime)
							{
								ExitCallback(*collisionResults[j].a, *collisionResults[j].b);
							}
						}
					}
					*/
					t = result.enterTime;
					BoxCollider2D& a = *result.a;
					BoxCollider2D& b = *result.b;

					if (!a.IsTrigger() && !b.IsTrigger())
					{
						
						ResolveContiniousRigidCollision(result,fixedDeltaTime);
						collisionResults.erase(collisionResults.begin() + i);
						std::vector<ContinuousCollision> recalculateResults;
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
									ContinuousCollisionDetect(
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
						TriggerCallback(a,b);
					}
					if (i == collisionResults.size() - 1||t>fixedDeltaTime)
					{
						allResolved = true;
					}
				}
			}	

		}
	
	/*	for (size_t i = 0; i < colliders.size(); i++)
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

				DiscreteCollisionDetect(a, b);
			}
		}*/
		
	}

	void CollisionManager::ResolveContiniousRigidCollision(ContinuousCollision& result,float fixedDeltaTime) {
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

		a.SetPosition(aCollidePos);
		b.SetPosition(bCollidePos);

		Vector3 contactPoint = CalculateContactPoint(a.GetBoundsAtPosition(aCollidePos), b.GetBoundsAtPosition(bCollidePos));
		result.collisionA.contactPoint = contactPoint;
		result.collisionB.contactPoint = contactPoint;

		CollisionCallback(a, b, result.collisionA, result.collisionB);

		float velocityAlongNormalA = result.collisionA.normal.Dot(aVelocity);
		float velocityAlongNormalB = result.collisionB.normal.Dot(bVelocity);
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

		if (rba)
		{
			rba->SetPosition(a.GetOwner().GetTransform().GetPosition() + (aVelocity * (fixedDeltaTime - result.enterTime )));
			rba->SetVelocity(aVelocity);
		//	std::cout << "normal x:" << result.collisionA.normal.x << " y: " << result.collisionA.normal.y << '\n';
		//	std::cout << "velocity x:" << aVelocity.x << " y: " << aVelocity.y << '\n';
		}
		if (rbb)
		{
			rbb->SetPosition(b.GetOwner().GetTransform().GetPosition() + (bVelocity * (fixedDeltaTime - result.enterTime )));
			rbb->SetVelocity(bVelocity);
		}
	}

	const std::array<Collision, 2>& CollisionManager::CalculateCollisionAndResolveOverlap(BoxCollider2D& a, BoxCollider2D& b) {

		Vector3 direction =a.GetWorldCenter() - b.GetWorldCenter();
		Vector3 separation;
		Bounds ba = a.GetBounds();
		Bounds bb = b.GetBounds();
		Vector3 contactPoint = CalculateContactPoint(ba,bb);
		if (direction.x > 0)
		{
			separation.x = bb.max.x - ba.min.x;
		}
		else
		{
			separation.x = bb.min.x - ba.max.x;
		}

		if (direction.y > 0)
		{
			separation.y = bb.max.y - ba.min.y;
		}
		else
		{
			separation.y = bb.min.y - ba.max.y;
		}

		Vector3 correctionVector = separation;
		if (std::abs(separation.x) < std::abs(separation.y))
			correctionVector.y = 0;
		else
			correctionVector.x = 0;
		float magnitude = correctionVector.Magnitude();
		ResolveCollision(a, b, correctionVector);
		Vector3 normal = correctionVector.Normalize();

		Vector3 aVelocity = a.GetRigidbody() ? a.GetRigidbody()->GetVelocity() : Vector3::Zero;
		Vector3 bVelocity = b.GetRigidbody() ? b.GetRigidbody()->GetVelocity() : Vector3::Zero;

		Vector3 relativeVeloctiy = aVelocity - bVelocity;

		Collision aCollision( b, normal , contactPoint,relativeVeloctiy );
		Collision bCollision(a, normal * (-1), contactPoint,relativeVeloctiy* (-1) );

		return { aCollision,bCollision };

	}

	void CollisionManager::DiscreteCollisionDetect(
		BoxCollider2D& a,
		BoxCollider2D& b)
	{
		Bounds ba = a.GetBounds();
		Bounds bb = b.GetBounds();
		bool overlap = CheckOverlapY(ba, bb,true) && CheckOverlapX(ba, bb,true);
		if (overlap)
		{	
			if (!a.IsTrigger() && !a.IsTrigger())
			{
				std::array<Collision, 2> collisions = CalculateCollisionAndResolveOverlap(a, b);
				CollisionCallback(a, b, collisions[0], collisions[1]);
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
			a.GetWorldCenter() + correctionVector * aRatio
		);

		b.SetPosition(
			b.GetWorldCenter() - correctionVector * bRatio
		);
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