#include "Image.h"
#include "Window.h"
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
}