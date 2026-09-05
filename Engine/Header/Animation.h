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
		Animation(const sf::Texture& texture):texture(texture){}

		void SetUpDefaultAnimation(std::vector<sf::IntRect> rects, int fps);
		void AddKey(AnimationKey key);
		void AddKeys(const std::vector<AnimationKey>& keys);
		const sf::Texture& GetTexture() const { return texture; }
		std::size_t GetKeyCount() const{return frames.size();}
		float GetFramePlayTime(int index) const { return frames[index].playTime; }
		float GetTransitionTime() const { return transitionTime; }
		const sf::IntRect& GetFrameRect(int index) const
		{
			return frames[index].textureRect;
		}
	private:
		const sf::Texture& texture;
		std::vector<AnimationKey> frames;
		float transitionTime = 0.05f;
	};
}