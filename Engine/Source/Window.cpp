#include "Window.h"
#include "BoxCollider2D.h"
#include "Bounds.h"
#include "EngineSettings.h"
#include "Vector3.h"
#include "Camera.h"
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

	void TinyEngine::Window::DrawCollider(const BoxCollider2D& collider, const Camera& camera)
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

		Vector3 camPosition = collider.GetPosition()- camera.GetPosition();
		Vector3 relativePixels{
		WorldToPixels(camPosition.x),
		WorldToPixels(camPosition.y),
		0.f
			};
		sf::Vector2u windowSize = sfmlWindow.getSize();

		Vector3 windowCenter{
			windowSize.x / 2.f,
			windowSize.y / 2.f,
			0.f
		};

		Vector3 objectScreenPosition{
			windowCenter.x + relativePixels.x,
			windowCenter.y + relativePixels.y,
			0.f
		};

		rectangle.setPosition({
			objectScreenPosition.x,
			objectScreenPosition.y
			});

		rectangle.setFillColor(sf::Color::Transparent);
		rectangle.setOutlineColor(sf::Color::Green);
		rectangle.setOutlineThickness(1.0f);

		Draw(rectangle);
	}
}