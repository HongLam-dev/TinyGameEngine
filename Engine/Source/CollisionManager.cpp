#include "CollisionManager.h"
#include "BoxCollider2D.h"
#include "Vector3.h"
#include "Bounds.h"
#include "RigidBody2D.h"
#include "Collision.h"
#include <array>
#include <cmath>
#include <iostream>
#include <vector>
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
		float tExit = fixedDeltaTime + 1;

		Vector3 relativeVelocity = aVelocity - bVelocity;
		bool overlapX = CheckOverlapX(a.GetBounds(), b.GetBounds());
		bool overlapY = CheckOverlapY(a.GetBounds(), b.GetBounds());

		if (relativeVelocity != Vector3::Zero || overlapX|| overlapY)
		{
			bool overlapped = false;

			Vector3 aPreviousPos = a.GetRigidbody() ? a.GetRigidbody()->GetPreviousPosition() : a.GetPosition();
			Vector3 bPreviousPos = b.GetRigidbody() ? b.GetRigidbody()->GetPreviousPosition() : b.GetPosition();

			Bounds aPreviousBounds = a.GetBoundsAtPosition(aPreviousPos);
			Bounds bPreviousBounds = b.GetBoundsAtPosition(bPreviousPos);

			float tEnterX = tEnter;
			float tExitX = tExit;
			float tEnterY= tEnter;
			float tExitY= tExit;


			if (relativeVelocity.x != 0)
			{
				tEnterX = (bPreviousBounds.min.x - aPreviousBounds.max.x) / relativeVelocity.x;
				tExitX = (bPreviousBounds.max.x - aPreviousBounds.min.x) / relativeVelocity.x;
			}
			else {
					tEnterX = overlapX? 0 : fixedDeltaTime+1;
			}

			if (relativeVelocity.y != 0)
			{
				tEnterY = (bPreviousBounds.min.y - aPreviousBounds.max.y) / relativeVelocity.y;
				tExitY= (bPreviousBounds.max.y - aPreviousBounds.min.y) / relativeVelocity.y;
			}
			else {
				tEnterY = overlapY ? 0 : fixedDeltaTime + 1;
			}

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
			if (tEnterX >= 0 && tEnterY >= 0)
			{
				tEnter = tEnterX > tEnterY ? tEnterX : tEnterY;
			}
			else {
				tEnter= fixedDeltaTime + 1;
			}
			tExit = tExitX < tExitY ? tExitX : tExitY;
		}
		bool hasCollision = false;
		if (tExit > 0 && tEnter < fixedDeltaTime && tEnter >= 0 && tEnter < tExit)
			hasCollision = true;



		return ContinuousCollision(tEnter,tExit,hasCollision,a,b);
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
				collisionResults.push_back(ContinuousCollisionDetect(a, b, fixedDeltaTime));
			}
		}

		if (!collisionResults.empty())
		{
			float t = 0;
			int trytime = 0;
			bool allResolved = false;
			while (t <= fixedDeltaTime&&!collisionResults.empty()&&!allResolved)
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
					ContinuousCollision& result = collisionResults[i];
					if (!result.hasCollision)
					{
						if(result.enterTime > fixedDeltaTime)
							t = result.enterTime;
						continue;
					}
			//		std::cout << result.enterTime << '\n';
					t = result.enterTime;
					BoxCollider2D& a = *result.a;
					BoxCollider2D& b = *result.b;

					if (!result.a->GetIsTrigger() && !result.b->GetIsTrigger())
					{
						Vector3 aVelocity = a.GetRigidbody() ? a.GetRigidbody()->GetVelocity() : Vector3::Zero;
						Vector3 bVelocity = b.GetRigidbody() ? b.GetRigidbody()->GetVelocity() : Vector3::Zero;

						Vector3 aPreviousPos = a.GetRigidbody() ? a.GetRigidbody()->GetPreviousPosition() : a.GetPosition();
						Vector3 bPreviousPos = b.GetRigidbody() ? b.GetRigidbody()->GetPreviousPosition() : b.GetPosition();

						Vector3 aCollidePos = aPreviousPos + aVelocity * t;
						Vector3 bCollidePos = bPreviousPos + bVelocity * t;

						a.SetPosition(aCollidePos);
						b.SetPosition(bCollidePos);

						std::array<Collision, 2> collisions = CalculateCollisionAndResolveOverlap(a, b);
						CollisionCallback(a, b, collisions[0], collisions[1]);

						float velocityAlongNormalA = collisions[0].normal.Dot(aVelocity);
						//std::cout << "normal y:" << collisions[0].normal.y << " x:" << collisions[0].normal.x << '\n';
						//std::cout << "vy:" << aVelocity.y << " vx:" << aVelocity.x << '\n';
						float velocityAlongNormalB = collisions[1].normal.Dot(bVelocity);

						if (velocityAlongNormalA < 0)
						{
							Vector3 normalVelocity = collisions[0].normal * velocityAlongNormalA;
							aVelocity -= normalVelocity;
						}
						if (velocityAlongNormalB < 0)
						{
							Vector3 normalVelocity = collisions[1].normal * velocityAlongNormalB;
							bVelocity -= normalVelocity;
						}

						if (a.GetRigidbody())
						{
							a.SetPosition(a.GetPosition() + (aVelocity * (fixedDeltaTime - t)));
							//	std::cout << "current velocity: y:" << aVelocity.y << " x:" << aVelocity.x << '\n';
							a.GetRigidbody()->SetVelocity(aVelocity);
						}
						if (b.GetRigidbody())
						{
							b.SetPosition(b.GetPosition() + (bVelocity * (fixedDeltaTime - t)));
							b.GetRigidbody()->SetVelocity(bVelocity);
						}
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
										fixedDeltaTime
									);

								recalculateResults.push_back(newCollision);

							}
							else
							{
								++col;
							}
						}
						i = 0;
						collisionResults.insert(collisionResults.end(), recalculateResults.begin(), recalculateResults.end());
					}
					else {
						TriggerCallback(a,b);
					}
					if (i == collisionResults.size() - 1)
						allResolved = true;
				}
			}

			for (size_t i = 0; i < collisionResults.size(); i++)
			{
				ContinuousCollision& result = collisionResults[i];
				if (result.exitTime<0 || result.exitTime>fixedDeltaTime)
					continue;

				BoxCollider2D& a = *result.a;
				BoxCollider2D& b = *result.b;

				if (result.a->GetIsTrigger() || result.b->GetIsTrigger())
				{
					ExitCallback(a, b);
				}
			}

		}
	
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

	const std::array<Collision, 2>& CollisionManager::CalculateCollisionAndResolveOverlap(BoxCollider2D& a, BoxCollider2D& b) {

		Vector3 direction =a.GetPosition() - b.GetPosition();
		Vector3 contactPoint=Vector3::Zero;
		Vector3 separation=Vector3::Zero;
		Bounds ba = a.GetBounds();
		Bounds bb = b.GetBounds();
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
		Vector3 normal = Vector3::Zero;
		if (separation.x != 0 && separation.y != 0)
		{
			Vector3 correctionVector = separation;
			if (std::abs(separation.x) < std::abs(separation.y))
				correctionVector.y = 0;
			else
				correctionVector.x = 0;
			float magnitude = correctionVector.Magnitude();
			ResolveCollision(a, b, correctionVector);
			normal = correctionVector.Normalize();
		}
		else {
			if (separation.x == 0)
			{
				if(direction.x>0)
					normal = { 1,0, 0 };
				else
					normal = { -1,0, 0 };
			}
			else if(separation.y == 0){
				if (direction.y > 0)
					normal = { 0,1, 0 };
				else
					normal = { 0,-1, 0 };
			}
		}

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
		bool overlap = CheckOverlapY(ba, bb) && CheckOverlapX(ba, bb);
		if (overlap)
		{	
			if (!a.GetIsTrigger() && !a.GetIsTrigger())
			{
				std::array<Collision, 2> collisions = CalculateCollisionAndResolveOverlap(a, b);
				CollisionCallback(a, b, collisions[0], collisions[1]);
			}
			else {
				TriggerCallback(a,b);
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
			a.min.x <= b.max.x &&
			a.max.x >= b.min.x;
		return overlapX;
	}
	bool CollisionManager::CheckOverlapY(const Bounds& a, const Bounds& b) {
		bool overlapY =
			a.min.y <= b.max.y &&
			a.max.y >= b.min.y;
		return overlapY;
	}
}