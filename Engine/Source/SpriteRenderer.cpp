#include "SpriteRenderer.h"
#include "Vector3.h"
namespace TinyEngine {
	void SpriteRenderer::Render(Window& window, Vector3 renderPos)
	{
		sprite->setPosition({ renderPos.x,renderPos.y });
		window.Draw(*sprite);
	}
	void SpriteRenderer::SetSprite(sf::Sprite& sprite)
	{
		this->sprite = &sprite;
	}
}