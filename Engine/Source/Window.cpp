#include "Window.h"
#include "BoxCollider2D.h"
#include "Bounds.h"
#include "EngineSettings.h"
#include "Vector3.h"
#include "Camera.h"
#include <iostream>
namespace TinyEngine
{

	bool Window::IsOpen()
	{
		return sfmlWindow.isOpen();
	}

	void Window::Close()
	{
		sfmlWindow.close();
	}

	std::optional<sf::Event> Window::PollEvent() {
		return sfmlWindow.pollEvent();
	}
	void Window::Display()
	{
		sfmlWindow.display();
	}

	void Window::Clear()
	{
		sfmlWindow.clear();
	}

	void Window::Draw(const sf::Drawable& object)
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

	void Window::DrawCollider(const BoxCollider2D& collider, const Camera& camera)
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

		Vector3 objectScreenPosition = camera.WorldToScreenPosition(collider.GetPosition(),GetSize());

		rectangle.setPosition({
			objectScreenPosition.x,
			objectScreenPosition.y
			});

		rectangle.setFillColor(sf::Color::Transparent);
		rectangle.setOutlineColor(sf::Color::Green);
		rectangle.setOutlineThickness(1.0f);

		Draw(rectangle);
	}

	Vector2 Window::GetSize() {
		sf::Vector2u windowSize = sfmlWindow.getSize();

		return{static_cast<float>(windowSize.x),static_cast<float>(windowSize.y) };
	}
}