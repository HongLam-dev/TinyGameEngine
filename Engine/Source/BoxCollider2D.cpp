#include "Vector3.h"
#include "TinyGameEngine.h"

namespace TinyEngine {

	BoxCollider2D::BoxCollider2D(GameObject& owner):Component(owner) {
	}
	
	void BoxCollider2D::Start() {
		SetRigidbody();
	}

	void BoxCollider2D::SetRigidbody() {
		rb = GetOwner().GetComponent<Rigidbody2D>();
	}

	Vector3 BoxCollider2D::GetPosition() const {
		return GetOwner().GetComponent<Transform>()->GetPosition()+offset;
	}
	Bounds BoxCollider2D::GetBounds() const {
		Bounds bounds;
		bounds.max = Vector3(GetPosition().x+size.x/2, GetPosition().y + size.y/2,0);
		bounds.min = Vector3(GetPosition().x - size.x/2, GetPosition().y - size.y/2, 0);
		return bounds;
	}

	void BoxCollider2D::SetPosition(const Vector3& newPos) {
		GetOwner().GetComponent<Transform>()->SetPosition(newPos-offset);
	}

	void BoxCollider2D::NotifyCollisionEnter(BoxCollider2D& other) {
		for (auto* component : GetOwner().GetAllComponents())
		{
			component->OnCollisionEnter(other);
		}
	}
	void BoxCollider2D::NotifyCollisionStay(BoxCollider2D& other) {
		for (auto* component : GetOwner().GetAllComponents())
		{
			component->OnCollisionStay(other);
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
}