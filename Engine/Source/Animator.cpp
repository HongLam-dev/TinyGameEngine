#include "Animator.h"
#include "Animation.h"
#include "TinyGameEngine.h"
 
namespace TinyEngine {

	void Animator::Start() {
		renderer = GetOwner().GetComponent<SpriteRenderer>();
		if (currentAnimation != nullptr && renderer != nullptr)
		{
			renderer->SetTexture(currentAnimation->GetTexture());
		}
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

	void Animator::SetAnimation(const Animation& animation) {
		currentAnimation = &animation; 
		if(renderer != nullptr)
			renderer->SetTexture(animation.GetTexture());
	}

	void Animator::Update() {
		if (currentAnimation != nullptr&&renderer!=nullptr)
		{	
			Play(*currentAnimation, GetOwner().GetEngineContext().GetDeltaTime());
			renderer->SetTextureRect(currentAnimation->GetFrameRect(currentFrame));
		}
	}
}