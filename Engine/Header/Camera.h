#pragma once
#include "GameObject.h"
#include "Component.h"
#include "Vector3.h"

namespace TinyEngine {
	class Camera : public Component
	{
	public:
		Camera(GameObject& owner):Component(owner) {}
		
		void Update() override;
		void SetPosition(const Vector3& pos) { GetTransform().SetPosition(pos); }
		const Vector3& GetPosition() const { return GetTransform().GetPosition(); }
	private:
	};

}