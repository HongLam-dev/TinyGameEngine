#include "SpriteRenderer.h"
#include "Vector3.h"
#include "Transform.h"
#include "EngineSettings.h"
#include "Camera.h"
#include <iostream>
namespace TinyEngine {

    void SpriteRenderer::Render(Window& window,const Camera& camera)
    {
        if (!sprite.get())
            return;
        Transform transform = GetTransform();

        Vector2 objectScreenPosition = camera.WorldToScreenPosition(transform.GetPosition(),window.GetSize());

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

    Bounds SpriteRenderer::GetBounds() {
        Bounds bounds;
        Vector3 position = GetTransform().GetPosition();
        Vector3 scale = GetTransform().GetScale();
        sf::Vector2i rectSize = sprite->getTextureRect().size;
        float width = rectSize.x * scale.x;
        float height = rectSize.y * scale.y;
        bounds.min.x = position.x - (width/2.0f);
        bounds.max.x = position.x + (width / 2.0f);
        bounds.min.y = position.y - (height / 2.0f);
        bounds.max.y = position.y + (height / 2.0f);

        return bounds;
    }

    void SpriteRenderer::OnDestroy() {
        GetOwner().UnregisterRenderer(*this);
    }
}