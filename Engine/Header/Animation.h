#pragma once
#include "Component.h"
#include <SFML/Graphics.hpp>
#include <vector>
namespace TinyEngine {
	class Animation
	{
	public:
		struct AnimationKey
		{
			sf::IntRect textureRect;
			float playTime = 0;
		};
	public:
		void SetUpDefaultAnimation(std::vector<sf::IntRect> rects, int fps);
		void AddKey(AnimationKey key);
		sf::IntRect GetCurrentFrame(int index) const { return frames[index].textureRect; }
		int GetKeyCount() const { return frames.size(); }
		float GetFramePlayTime(int index) const { return frames[index].playTime; }
		float GetTransitionTime() const { return transitionTime; }
		sf::IntRect GetFrameRect(int index) { return frames[index].textureRect; }
	private:
		const sf::Texture* texture=nullptr;
		std::vector<AnimationKey> frames;
		float transitionTime = 0.5f;
	};
}