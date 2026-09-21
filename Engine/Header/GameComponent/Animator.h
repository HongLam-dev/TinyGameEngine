#pragma once
#include "Component.h"
#include "Animation.h"
#include "SpriteRenderer.h"
#include <vector>
#include <memory>

namespace TinyEngine {
	class Animator: public Component
	{
	public:
		Animator(GameObject& owner):Component(owner){}
		void AddAnimation(std::string name,Animation&& animation);
		void SetAnimation(std::string name);
		void Play(const Animation& animation, float deltaTime);
		void Start() override;
		void Update(float deltaTime) override;
	private:
		float timer = 0;
		int currentFrame = 0;
		SpriteRenderer* renderer = nullptr;
		std::string currentAnimation = "";
		std::unordered_map<std::string, Animation> animations;
	};
}