#pragma once
#include "Window.h"
#include "TextureManager.h"
#include "Transform.h"
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "Camera.h"
#include "RenderableComponent.h"
#include <string>

namespace TinyEngine {

    class SpriteRenderer : public RenderableComponent
    {
    public:
        Bounds GetBounds();
	    void Render(Window& window, const Camera& camera);
        void OnDestroy() override;
        SpriteRenderer(GameObject& owner):RenderableComponent(owner) {
            GetOwner().RegisterRenderer(*this);
        }
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
