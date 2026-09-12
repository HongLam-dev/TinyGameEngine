#include "TextureManager.h"
#include <iostream>
namespace TinyEngine {
	sf::Texture* TextureManager::GetTexture(std::string path) {
		if (!textures.contains(path))
		{
			if (!textures[path].loadFromFile(path)) {
				std::cout << path << " not found \n";
				return nullptr;
			}
		}
		return &textures[path];
	}

}