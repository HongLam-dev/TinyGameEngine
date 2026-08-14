#pragma once
#include "Component.h"
#include "Window.h"
#include "Vector3.h"
#include <SFML/Graphics.hpp>
#include "GameObject.h"

namespace TinyEngine {

    class SpriteRenderer : public Component
    {
    public:
	    void Render(Window& window, const Vector3& renderPos);
	    void SetSprite(sf::Sprite& sprite);
        SpriteRenderer(GameObject& owner):Component(owner) {}
    private:
	    sf::Sprite* sprite = nullptr;;
    };

}
