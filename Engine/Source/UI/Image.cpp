#include "Image.h"
#include "Window.h"
#include "Bounds.h"
#include <iostream>

namespace TinyEngine{
	void Image::Render(Window& window) {
        if (!sprite.get())
            return;
        Transform transform = GetTransform();

        sprite->setPosition({
            transform.GetPosition().x,
            transform.GetPosition().y
            });

        sprite->setScale({
            transform.GetScale().x,
            transform.GetScale().y
            });

        sprite->setOrigin({
    sprite->getLocalBounds().size.x / 2.0f,
    sprite->getLocalBounds().size.y / 2.0f
            });

        sf::Angle angle = sf::radians(
            std::atan2(
                transform.GetRotation().x,
                transform.GetRotation().y
            )
        );

        sprite->setRotation(angle);

        window.Draw(*sprite);
	}

    void Image::SetTexture(std::string texturePath)
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


    Bounds Image::GetBounds() {
        Bounds bounds;
        if (sprite)
        {
            Vector3 position = GetTransform().GetPosition();
            Vector3 scale = GetTransform().GetScale();
            sf::Vector2i rectSize = sprite->getTextureRect().size;
            float width = rectSize.x * scale.x;
            float height = rectSize.y * scale.y;
            bounds.min.x = position.x - (width / 2.0f);
            bounds.max.x = position.x + (width / 2.0f);
            bounds.min.y = position.y - (height / 2.0f);
            bounds.max.y = position.y + (height / 2.0f);
        }
        return bounds;
    }
}