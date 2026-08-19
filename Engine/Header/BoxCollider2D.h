#pragma once
#include "Component.h"
#include "GameObject.h"
#include "Vector3.h"
#include "Bounds.h"
namespace TinyEngine {
	class BoxCollider2D: public Component
	{
	public:
		BoxCollider2D(GameObject& owner);
			
		void SetSize(const Vector3& size)  { this->size=size; }
		void SetStatic(bool isStatic) { this->isStatic = isStatic; }
		void SetPosition(const Vector3& newPos);
		Vector3 GetPosition() const;
		Vector3 GetSize() const { return size; }
		Bounds GetBounds() const;
		bool IsStatic() { return isStatic; }
	private:
		Vector3 size{1,1,1};
		Vector3 offset = Vector3::Zero;
		bool isStatic = true;
		bool isTrigger = false;
	};

}