#pragma once
#include "GameObject.h"
#include "Component.h"

namespace TinyEngine {
	class CameraFollow:public Component
	{
	public:
		CameraFollow(GameObject& owner):Component(owner) {};
		void SetTarget(Transform& target) { this->target = &target; }
		void Update() override;
	private:
		Transform* target = nullptr;
	};

}