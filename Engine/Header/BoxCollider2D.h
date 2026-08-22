#pragma once
#include "Component.h"
#include "Vector3.h"
#include "Rigidbody2D.h"
#include "Bounds.h"
namespace TinyEngine {
	class GameObject;
	class BoxCollider2D: public Component
	{
	public:
		BoxCollider2D(GameObject& owner);
		void Start() override;
		void SetRigidbody();
		void NotifyCollisionEnter(BoxCollider2D& other);
		void NotifyCollisionStay(BoxCollider2D& other);
		void NotifyCollisionExit(BoxCollider2D& other);
		void NotifyTriggerEnter(BoxCollider2D& other);
		void NotifyTriggerStay(BoxCollider2D& other);
		void NotifyTriggerExit(BoxCollider2D& other);
		Rigidbody2D* GetRigidbody() { return rb; }
		void SetSize(const Vector3& size)  { this->size=size; }
		void SetPosition(const Vector3& newPos);
		Vector3 GetPosition() const;
		Vector3 GetSize() const { return size; }
		Bounds GetBounds() const;
		void SetIsTrigger(bool isTrigger) {
			this->isTrigger = isTrigger;
		}
		bool GetIsTrigger() { return isTrigger; };
	private:
		Vector3 size{1,1,1};
		Vector3 offset = Vector3::Zero;
		Rigidbody2D* rb = nullptr;
		bool isTrigger = false;
	};

}