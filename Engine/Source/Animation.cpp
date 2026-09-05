#include "Animation.h"
namespace TinyEngine {
	void Animation::Play(float deltaTime)
	{
		elapsedTime += deltaTime;
		if (currentFrame < frames.size()-1)
		{
			if (elapsedTime >= frames[currentFrame + 1].playTime)
			{
				currentFrame++;
			}
		}
		else {
			if (elapsedTime > frames.end()->playTime+ transitionTime)
			{
				elapsedTime = 0;
				currentFrame = 0;
			}
		}
	}

	void  Animation::SetUpDefaultAnimation(std::vector<sf::IntRect> rects, int fps) {
		float playTime = 0;
		float duration = 1.0f / fps;
		for (auto& rect : rects)
		{
			AnimationKey key{rect,playTime};
			AddKey(key);
			playTime += duration;
		}
	}

	void Animation::AddKey(AnimationKey key)
	{
		frames.push_back(key);
		std::sort(frames.begin(), frames.end(),
			[](const AnimationKey& a, const AnimationKey& b)
			{
				return a.playTime< b.playTime;
			});
	}
}