#include "Vector3.h"
#include "TinyGameEngine.h"

namespace TinyEngine {

	BoxCollider2D::BoxCollider2D(GameObject& owner):Component(owner) {
		GetOwner().RegisterCollider(*this);
	}

	void BoxCollider2D::Start() {
		
		SetRigidbody();
	}

	void BoxCollider2D::SetRigidbody() {
		rb = GetOwner().GetComponent<Rigidbody2D>();
	}

	Vector3 BoxCollider2D::GetPosition() const {
		return GetTransform().GetPosition()+offset;
	}
	Bounds BoxCollider2D::GetBounds() const {
		return GetBoundsAtPosition(GetTransform().GetPosition());
	}

	Bounds BoxCollider2D::GetBoundsAtPosition(Vector3 position) const {
		Bounds bounds;
		Vector3 scale = GetTransform().GetScale();
		Vector3 colliderPosition = position + offset;
		float width = size.x * scale.x;
		float height = size.y * scale.y;
		bounds.max = Vector3(colliderPosition.x + width / 2, colliderPosition.y + width / 2, 0);
		bounds.min = Vector3(colliderPosition.x - height / 2, colliderPosition.y - height / 2, 0);
		return bounds;
	}

	void BoxCollider2D::SetPosition(const Vector3& newPos) {
		GetTransform().SetPosition(newPos-offset);
	}

	void BoxCollider2D::NotifyCollisionEnter(const Collision& collision) {
		for (auto* component : GetOwner().GetAllComponents())
		{
			component->OnCollisionEnter(collision);
		}
	}
	void BoxCollider2D::NotifyCollisionStay(const Collision& collision) {
		for (auto* component : GetOwner().GetAllComponents())
		{
			component->OnCollisionStay(collision);
		}
	}
	void BoxCollider2D::NotifyCollisionExit(BoxCollider2D& other) {
		for (auto* component : GetOwner().GetAllComponents())
		{
			component->OnCollisionExit(other);
		}
	}
	void BoxCollider2D::NotifyTriggerEnter(BoxCollider2D& other) {
		for (auto* component : GetOwner().GetAllComponents())
		{
			component->OnTriggerEnter(other);
		}
	}
	void BoxCollider2D::NotifyTriggerStay(BoxCollider2D& other) {
		for (auto* component : GetOwner().GetAllComponents())
		{
			component->OnTriggerStay(other);
		}
	}
	void BoxCollider2D::NotifyTriggerExit(BoxCollider2D& other) {
		for (auto* component : GetOwner().GetAllComponents())
		{
			component->OnTriggerExit(other);
		}
	}
	void BoxCollider2D::OnDestroy() {
		GetOwner().UnregisterCollider(*this);
	}
}