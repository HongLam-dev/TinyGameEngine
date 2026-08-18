#include "BoxCollider2D.h"
#include "Vector3.h"
#include "TinyGameEngine.h"

namespace TinyEngine {

	BoxCollider2D::BoxCollider2D(GameObject& owner):Component(owner) {
		GetOwner().GetEngineContext().GetCollisionManager().AsignCollider(this);
	}

	Vector3 BoxCollider2D::GetPosition() const {
		return GetOwner().GetComponent<Transform>().GetPosition()+offset;
	}
	Bounds BoxCollider2D::GetBounds() const {
		Bounds bounds;
		bounds.max = Vector3(GetPosition().x+size.x/2, GetPosition().y + size.y/2,0);
		bounds.min = Vector3(GetPosition().x - size.x/2, GetPosition().y - size.y/2, 0);
		return bounds;
	}

	void BoxCollider2D::SetPosition(const Vector3& newPos) {
		GetOwner().GetComponent<Transform>().SetPosition(newPos-offset);
	}
}