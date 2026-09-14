#pragma once
#include "Window.h"
#include "Transform.h"
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "Camera.h"
#include "RenderableComponent.h"

namespace TinyEngine {

    class SpriteRenderer : public RenderableComponent
    {
    public:
	    void Render(Window& window, const Camera& camera);
        void OnDestroy() override;
        SpriteRenderer(GameObject& owner):RenderableComponent(owner) {
            GetOwner().RegisterRenderer(*this);
        }
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
    private:
        std::unique_ptr< sf::Sprite> sprite;
    };

}
