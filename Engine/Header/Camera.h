#pragma once
#include "GameObject.h"
#include "Component.h"
#include "Vector3.h"
#include "Vector2.h"

namespace TinyEngine {
	class Camera : public Component
	{
	public:
		Camera(GameObject& owner):Component(owner) {}
		const Vector2& WorldToScreenPosition(Vector3 worldPos, Vector2 windowSize) const;
		void Update(float deltaTime) override;
		void SetPosition(const Vector3& pos) { GetTransform().SetPosition(pos); }
		const Vector3& GetPosition() const { return GetTransform().GetPosition(); }
	private:
	};

}