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
		void Play(float deltaTime);
		void SetUpDefaultAnimation(std::vector<sf::IntRect> rects, int fps);
		void AddKey(AnimationKey key);
		sf::IntRect GetCurrentFrame() const { return frames[currentFrame].textureRect; }
	private:
		const sf::Texture* texture=nullptr;
		std::vector<AnimationKey> frames;
		int currentFrame = 0;
		float transitionTime = 0.5f;
		float elapsedTime = 0;
	};
}