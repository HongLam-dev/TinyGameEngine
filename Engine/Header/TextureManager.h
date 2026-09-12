#pragma once
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
namespace TinyEngine {
	class TextureManager
	{
	public:
		sf::Texture* GetTexture(std::string path);
	private:
		std::unordered_map<std::string,sf::Texture> textures;
	};
}