#pragma once
#include "Component.h"
#include "Animation.h"
#include "SpriteRenderer.h"

namespace TinyEngine {
	class Animator: public Component
	{
	public:
		Animator(GameObject& owner):Component(owner){}
		void SetAnimation(const Animation& animation);
		void Play(const Animation& animation, float deltaTime);
		void Start() override;
		void Update() override;
	private:
		float timer = 0;
		int currentFrame = 0;
		SpriteRenderer* renderer = nullptr;
		const Animation* currentAnimation = nullptr;
	};
}