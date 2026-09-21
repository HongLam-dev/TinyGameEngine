#include "Animator.h"
#include "Animation.h"
#include "TinyGameEngine.h"
 
namespace TinyEngine {

	void Animator::Start() {
		renderer = GetOwner().GetComponent<SpriteRenderer>();
	}

	void Animator::Play(const Animation& animation,float deltaTime)
	{
		timer += deltaTime;
		if (currentFrame < animation.GetKeyCount() - 1)
		{
			if (timer >= animation.GetFramePlayTime(currentFrame + 1))
			{
				currentFrame++;
			}
		}
		else {
			if (timer > animation.GetFramePlayTime(currentFrame) + animation.GetTransitionTime())
			{
				timer = 0;
				currentFrame = 0;
			}
		}
	}

	void Animator::SetAnimation(std::string name) {
		currentAnimation = name;
		if (renderer != nullptr && animations.contains(name))
			renderer->SetTexture(animations.at(name).GetTexture());
		else
			std::cout<<"Animation "<<name << " not found\n";
	}

	void Animator::Update(float deltaTime) {
		if (animations.contains(currentAnimation) && renderer != nullptr)
		{	
			Play(animations.at(currentAnimation), deltaTime);
			renderer->SetTextureRect(animations.at(currentAnimation).GetCurrentFrameRect(currentFrame));
		}
		else
		{
			std::cout << "Animation " << currentAnimation << " not found \n";
		}
	}

	void Animator::AddAnimation(std::string name, Animation&& animation) {
		animations.emplace(std::move(name), std::move(animation));
	}
	
}