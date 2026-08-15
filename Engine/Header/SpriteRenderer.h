#pragma once
#include "Component.h"
#include "Window.h"
#include "Transform.h"
#include <SFML/Graphics.hpp>
#include "GameObject.h"

namespace TinyEngine {

    class SpriteRenderer : public Component
    {
    public:
	    void Render(Window& window, const Transform& transform);
	    void SetSprite(sf::Sprite& sprite);
        SpriteRenderer(GameObject& owner):Component(owner) {}
    private:
	    sf::Sprite* sprite = nullptr;;
    };

}
