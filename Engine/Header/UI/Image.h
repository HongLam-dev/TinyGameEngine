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
            sf::Texture* tex =
                TextureManager::Instance().GetTexture(texturePath);

            if (!tex)
            {
                return;
            }

            if (sprite == nullptr)
            {
                sprite = std::make_unique<sf::Sprite>(*tex);
            }
            else
            {
                sprite->setTexture(*tex);
            }

            sprite->setTextureRect(
                sf::IntRect(
                    { 0, 0 },
            {
                static_cast<int>(tex->getSize().x),
                static_cast<int>(tex->getSize().y)
            }
                )
            );

            this->texturePath = texturePath;
        }

        std::string GetTexturePath() const
        {
            return texturePath;
        }

        void SetTextureRect(sf::IntRect rect)
        {
            if (sprite == nullptr)
                return;

            sprite->setTextureRect(rect);
        }

        sf::IntRect GetTextureRect() const
        {
            if (!sprite)
                return {};

            return sprite->getTextureRect();
        }
        void Render(Window& window) override;
	private:
        std::string texturePath;
		std::unique_ptr<sf::Sprite> sprite = nullptr;
	};
}