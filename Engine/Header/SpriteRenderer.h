#pragma once
#include "Component.h"
#include "Window.h"
#include "Vector3.h"
#include <SFML/Graphics.hpp>

namespace TinyEngine {

    class SpriteRenderer : public Component
    {
    public:
	    void Render(Window& window,Vector3 renderPos);
	    void SetSprite(sf::Sprite& sprite);
        SpriteRenderer() {}
    private:
	    sf::Sprite* sprite = nullptr;;
    };

}
