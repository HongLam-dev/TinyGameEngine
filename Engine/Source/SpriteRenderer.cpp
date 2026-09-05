#include "SpriteRenderer.h"
#include "Vector3.h"
#include "Transform.h"
#include "EngineSettings.h"
#include <iostream>
namespace TinyEngine {
    void SpriteRenderer::Render(
        Window& window,
        const Transform& transform)
    {
        if (!sprite.get())
            return;
        const Vector3 position = transform.GetPosition();
        sprite->setPosition({
            WorldToPixels(position.x),
            WorldToPixels(position.y)
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