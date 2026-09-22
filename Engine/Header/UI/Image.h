#pragma once
#include "TextureManager.h"
#include "Window.h"
#include "RenderableComponent.h"
#include "UIRenderable.h"
#include <string>
#include <SFML/Graphics.hpp>

namespace TinyEngine {
	class Image:public UIRenderable
	{
	public:
        using UIRenderable::UIRenderable;
        void SetTexture(std::string texturePath)
        {
            sf::Texture* tex = TextureManager::Instance().GetTexture(texturePath);
            if (!tex)
            {
                std::cout << "No texture found: " << texturePath;
                return;
            }
            if (sprite == nullptr)
            {
                sprite = std::make_unique<sf::Sprite>(*tex);
            }
            else
                sprite->setTexture(*tex);
            this->texturePath = texturePath;
        }
        void SetTextureRect(const sf::IntRect& rect)
        {
            if (sprite == nullptr)
                return;
            sprite->setTextureRect(rect);
        }
        void Render(Window& window) override;
	private:
        std::string texturePath;
		std::unique_ptr<sf::Sprite> sprite = nullptr;
	};
}