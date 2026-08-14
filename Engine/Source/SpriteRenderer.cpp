#include "SpriteRenderer.h"
#include "Vector3.h"
#include <iostream>
namespace TinyEngine {
	void SpriteRenderer::Render(Window& window, const Vector3& renderPos)
	{
		sprite->setPosition({ renderPos.x,renderPos.y });
		window.Draw(*sprite);
	}
	void SpriteRenderer::SetSprite(sf::Sprite& sprite)
	{
		this->sprite = &sprite;
	}
}