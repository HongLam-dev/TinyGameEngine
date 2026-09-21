#pragma once
#include "Component.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
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
		Animation(std::string texturePath):texturePath(texturePath){}
		void SetUpDefaultAnimation(std::vector<sf::IntRect> rects, int fps);
		void AddKey(AnimationKey key);
		void AddKeys(const std::vector<AnimationKey>& keys);
		std::string GetTexture() const { return texturePath; }
		std::size_t GetKeyCount() const{return frames.size();}
		float GetFramePlayTime(int index) const { return frames[index].playTime; }
		float GetTransitionTime() const { return transitionTime; }
		const sf::IntRect& GetCurrentFrameRect(int index) const
		{
			return frames[index].textureRect;
		}
	private:
		std::string texturePath;
		std::vector<AnimationKey> frames;
		float transitionTime = 0.05f;
	};
}