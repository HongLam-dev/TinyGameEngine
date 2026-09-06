#include "SpriteRenderer.h"
#include "Vector3.h"
#include "Transform.h"
#include "EngineSettings.h"
#include "Camera.h"
#include <iostream>
namespace TinyEngine {
    void SpriteRenderer::Render(
        Window& window,const Camera& camera,
        const Transform& transform)
    {
        if (!sprite.get())
            return;

        Vector3 camPosition = transform.GetPosition() - camera.GetPosition();
        Vector3 relativePixels{
        WorldToPixels(camPosition.x),
        WorldToPixels(camPosition.y),
        0.f
        };
        sf::Vector2u windowSize = window.GetSize();

        Vector3 windowCenter{
            windowSize.x / 2.f,
            windowSize.y / 2.f,
            0.f
        };

        Vector3 objectScreenPosition{
            windowCenter.x + relativePixels.x,
            windowCenter.y + relativePixels.y,
            0.f
        };

        sprite->setPosition({
            objectScreenPosition.x,
            objectScreenPosition.y
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