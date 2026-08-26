#include "Window.h"
#include "BoxCollider2D.h"
#include "Bounds.h"
#include "EngineSettings.h"
#include "Vector3.h"
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

	void Window::SetPosition(const Vector3& position)
	{
		
		sfmlWindow.setPosition({
			 static_cast<int>(position.x),
	static_cast<int>(position.y)
			});
	}

	void TinyEngine::Window::DrawCollider(const BoxCollider2D& collider)
	{
		const Bounds bounds = collider.GetBounds();

		sf::RectangleShape rectangle;

		const float width =
			WorldToPixels(bounds.max.x - bounds.min.x);

		const float height =
			WorldToPixels(bounds.max.y - bounds.min.y);

		rectangle.setSize({
			width,
			height
			});

		rectangle.setOrigin({
			width / 2.0f,
			height / 2.0f
			});

		const Vector3 position = collider.GetPosition();

		rectangle.setPosition({
			WorldToPixels(position.x),
			WorldToPixels(position.y)
			});

		rectangle.setFillColor(sf::Color::Transparent);
		rectangle.setOutlineColor(sf::Color::Green);
		rectangle.setOutlineThickness(1.0f);

		Draw(rectangle);
	}
}