#include "SpriteRenderer.h"
#include "Vector3.h"
#include "Transform.h"
#include <iostream>
namespace TinyEngine {
	void SpriteRenderer::Render(Window& window, const Transform& transform)
	{
		sprite->setPosition({ transform.GetPosition().x,transform.GetPosition().y});
		sprite->setScale({ transform.GetScale().x,transform.GetScale().y });
		sf::Angle angle = sf::radians(
			std::atan2(transform.GetRotation().x, transform.GetRotation().y)
		);
		sprite->setRotation(angle);
		window.Draw(*sprite);
	}
	void SpriteRenderer::SetSprite(sf::Sprite& sprite)
	{
		this->sprite = &sprite;
	}
}