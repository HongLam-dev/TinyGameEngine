#pragma once
#include "GameObject.h"
#include "Component.h"
namespace TinyGame {
	class CameraFollow:public TinyEngine::Component
	{
	public:
		CameraFollow(TinyEngine::GameObject& owner):Component(owner) {};
		void SetTarget(TinyEngine::Transform& target) { this->target = &target; }
		void Update(float deltaTime) override;
	private:
		TinyEngine::Transform* target = nullptr;
	};

}