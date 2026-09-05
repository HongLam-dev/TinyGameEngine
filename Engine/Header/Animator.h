#pragma once
#include "Component.h"
#include "Animation.h"
#include "SpriteRenderer.h"

namespace TinyEngine {
	class Animator: public Component
	{
	public:
		void SetAnimation(const Animation& animation) { currentAnimation = &animation; }
		void Play(const Animation& animation, float deltaTime);
		void Update() override;
	private:
		float timer = 0;
		int currentFrame = 0;
		const SpriteRenderer* renderer = nullptr;
		const Animation* currentAnimation;
	};
}