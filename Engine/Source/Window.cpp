#include "Window.h"
#include "BoxCollider2D.h"
#include "Bounds.h"
#include "EngineSettings.h"
#include <iostream>
namespace TinyEngine
{

	bool TinyEngine::Window::IsOpen()
	{
		return sfmlWindow.isOpen();
	}

	void TinyEngine::Window::Close()
	{
		sfmlWindow.close();
	}

	std::optional<sf::Event> TinyEngine::Window::PollEvent() {
		return sfmlWindow.pollEvent();
	}
	void TinyEngine::Window::Display()
	{
		sfmlWindow.display();
	}

	void TinyEngine::Window::Clear()
	{
		sfmlWindow.clear();
	}

	void TinyEngine::Window::Draw(const sf::Drawable& object)
	{
		sfmlWindow.draw(object);
	}

	void TinyEngine::Window::DrawCollider(const BoxCollider2D& collider)
	{
		const Bounds bounds = collider.GetBounds();

		sf::RectangleShape rectangle;

		rectangle.setPosition({
			WorldToPixels(bounds.min.x),
			WorldToPixels(bounds.min.y)
			});

		rectangle.setSize({
			WorldToPixels(bounds.max.x - bounds.min.x),
			WorldToPixels(bounds.max.y - bounds.min.y)
			});

		rectangle.setFillColor(sf::Color::Transparent);
		rectangle.setOutlineColor(sf::Color::Green);
		rectangle.setOutlineThickness(1.f);

		sfmlWindow.draw(rectangle);
	}
}