#include "Animator.h"
#include "Animation.h"
#include "TinyGameEngine.h"
 
namespace TinyEngine {
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

	void Animator::Update() {
		if (currentAnimation != nullptr)
		{	
			Play(*currentAnimation, GetOwner().GetEngineContext().GetDeltaTime());
		}
	}
}