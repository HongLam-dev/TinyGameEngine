#pragma once
#include <SFML/Graphics.hpp>
#include "Window.h"
#include "RenderableComponent.h"

namespace TinyEngine {
	class Image:public RenderableComponent
	{
	public:
		Image(GameObject& owner): RenderableComponent(owner){}
        void SetTexture(const sf::Texture& texture)
        {
            if (sprite == nullptr)
            {
                sprite = std::make_unique<sf::Sprite>(texture);
            }
            else
                sprite->setTexture(texture);
        }
        void SetTextureRect(const sf::IntRect& rect)
        {
            if (sprite == nullptr)
                return;
            sprite->setTextureRect(rect);
        }
        void Render(Window& window);
	private:
		std::unique_ptr<sf::Sprite> sprite = nullptr;
	};
}